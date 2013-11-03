
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "host.h"
#include "misc.h"
#include "machine.h"
#include "regs.h"
#include "memory.h"
#include "loader.h"
#include "syscall.h"
#include "dlite.h"
#include "options.h"
#include "stats.h"
#include "sim.h"
#include "decode.def"

#include "instr.h"

/* PARAMETERS OF THE TOMASULO'S ALGORITHM */

#define INSTR_QUEUE_SIZE         10

#define RESERV_INT_SIZE    4
#define RESERV_FP_SIZE     2
#define FU_INT_SIZE        2
#define FU_FP_SIZE         1

#define FU_INT_LATENCY     4
#define FU_FP_LATENCY      9

/* IDENTIFYING INSTRUCTIONS */

//unconditional branch, jump or call
#define IS_UNCOND_CTRL(op) (MD_OP_FLAGS(op) & F_CALL || \
                         MD_OP_FLAGS(op) & F_UNCOND)

//conditional branch instruction
#define IS_COND_CTRL(op) (MD_OP_FLAGS(op) & F_COND)

//floating-point computation
#define IS_FCOMP(op) (MD_OP_FLAGS(op) & F_FCOMP)

//integer computation
#define IS_ICOMP(op) (MD_OP_FLAGS(op) & F_ICOMP)

//load instruction
#define IS_LOAD(op)  (MD_OP_FLAGS(op) & F_LOAD)

//store instruction
#define IS_STORE(op) (MD_OP_FLAGS(op) & F_STORE)

//trap instruction
#define IS_TRAP(op) (MD_OP_FLAGS(op) & F_TRAP)

#define USES_INT_FU(op) (IS_ICOMP(op) || IS_LOAD(op) || IS_STORE(op))
#define USES_FP_FU(op) (IS_FCOMP(op))

#define WRITES_CDB(op) (IS_ICOMP(op) || IS_LOAD(op) || IS_FCOMP(op))

/* FOR DEBUGGING */

//prints info about an instruction
#define PRINT_INST(out,instr,str,cycle)	\
  myfprintf(out, "%d: %s", cycle, str);		\
  md_print_insn(instr->inst, instr->pc, out); \
  myfprintf(stdout, "(%d)\n",instr->index);

#define PRINT_REG(out,reg,str,instr) \
  myfprintf(out, "reg#%d %s ", reg, str);	\
  md_print_insn(instr->inst, instr->pc, out); \
  myfprintf(stdout, "(%d)\n",instr->index);

/* E552 Assignment 4 - BEGIN CODE */

/* Constants for magic numbers */
#define MAX_INPUT_REGS 3
#define MAX_OUTPUT_REGS 2

/* Helper function for working with arrays of instructions */
// Check if an array of insns is empty
int insn_array_is_empty(instruction_t** insns, int size) {
  assert(insns && size >= 0);
  int i;
  for(i = 0; i < size; i++) {
    if(insns[i]) return 0;
  }
  return 1;
}

// Check if an array of insns is full
int insn_array_is_full(instruction_t** insns, int size) {
  assert(insns && size >= 0);
  int i;
  for(i = 0; i < size; i++) {
    if(!insns[i]) return 0;
  }
  return 1;
}

// Get the oldest insn in an array of insns
instruction_t* insn_array_get_oldest(instruction_t** insns, int size) {
  int oldest_index = 0x7FFFFFFF;
  instruction_t* oldest_insn = NULL;
  int i;
  for(i = 0; i < size; i++) {
    instruction_t* insn = insns[i];
    if(insn && insn->index < oldest_index) {
      oldest_index = insn->index;
      oldest_insn = insn;
    }
  }
  return oldest_insn;
}

// Get the oldest insn in an array of insns
// Modified to get oldest instruction ready for execute (all inputs ready)
instruction_t* insn_array_get_oldest_ready_for_execute(
  instruction_t** insns,
  int size
) {
  int oldest_index = 0x7FFFFFFF;
  instruction_t* oldest_insn = NULL;
  int i;
  for(i = 0; i < size; i++) {
    instruction_t* insn = insns[i];
    if(!insn) continue;

    // Check if all inputs are available (or in other words, Q array is empty)
    if (!insn_array_is_empty(insn->Q, MAX_INPUT_REGS)) continue;

    if(insn->index < oldest_index) {
      oldest_index = insn->index;
      oldest_insn = insn;
    }
  }
  return oldest_insn;
}

// Get the oldest insn in an array of insns
// Modified to get oldest instruction ready for CDB (finish execution latency)
instruction_t* insn_array_get_oldest_ready_for_CDB(
  instruction_t** insns,
  int size,
  int current_cycle
) {
  int oldest_index = 0x7FFFFFFF;
  instruction_t* oldest_insn = NULL;
  int i;
  for(i = 0; i < size; i++) {
    instruction_t* insn = insns[i];
    if(!insn) continue;

    // We don't want anyone calling this function if the instruction hasn't
    // even executed yet
    assert(insn->tom_execute_cycle != 0);

    // Check if insn is finished execution based on FU latency
    int latency = 0;
    if(USES_INT_FU(insn->op)) {
      latency = FU_INT_LATENCY;
    } else if(USES_FP_FU(insn->op)) {
      latency = FU_FP_LATENCY;
    }
    if (insn->tom_execute_cycle + latency > current_cycle) continue;

    if(insn->index < oldest_index) {
      oldest_index = insn->index;
      oldest_insn = insn;
    }
  }
  return oldest_insn;
}

// Insert specified insn into array
// Inserts into first empty slot, else don't insert anything at all
void insn_array_insert_insn(
  instruction_t* insn,
  instruction_t** insns,
  int size
) {
  assert(insn && insns && size >= 0);
  int i;
  for(i = 0; i < size; i++) {
    if(!insns[i]) {
      insns[i] = insn;
      return;
    }
  }
}

// Remove specified insn from array
// Won't remove anything if the instruction isn't there
void insn_array_remove_insn(
  instruction_t* insn,
  instruction_t** insns,
  int size
) {
  assert(insn && insns && size >= 0);
  int i;
  for(i = 0; i < size; i++) {
    if(insns[i] == insn) {
      insns[i] = NULL;
      return;
    }
  }
}

// Use for debugging reservation stations
void print_insn_dependencies(instruction_t* insn, int current_cycle) {
  if(!insn) return;
  printf("********** Q DEPENDENCIES FOR ");
  md_print_insn(insn->inst, insn->pc, stdout);
  printf("**********\n");
  int i;
  for(i = 0; i < MAX_INPUT_REGS; i++) {
    if(!(insn->Q[i])) continue;
    PRINT_INST(stdout, insn->Q[i], "", current_cycle);
  }
  printf("\n");
}

/* E552 Assignment 4 - END CODE */

/* VARIABLES */

//instruction queue for tomasulo
static instruction_t* instr_queue[INSTR_QUEUE_SIZE];
//number of instructions in the instruction queue
static int instr_queue_size = 0;

/* E552 Assignment 4 - BEGIN CODE */
// Use a circular queue for instr_queue since instructions have to be
// dispatched in program order
static int instr_queue_head = 0;

/* Helper functions for instruction queue */
// Check if there's a next instruction
// Return 1 for true, 0 for false
int instr_queue_is_empty() {
  return instr_queue_size == 0 ? 1 : 0;
}

// Check if the instr queue is full
// Return 1 for true, 0 for false
int instr_queue_is_full() {
  return instr_queue_size == INSTR_QUEUE_SIZE;
}

// Peek at the next instruction
instruction_t* instr_queue_peek() {
  return instr_queue[instr_queue_head];
}

// Get the next instruction
instruction_t* instr_queue_dequeue() {
  assert(instr_queue_size != 0);
  instruction_t* insn = instr_queue[instr_queue_head];
  instr_queue[instr_queue_head] = NULL;
  instr_queue_head = (instr_queue_head + 1) % INSTR_QUEUE_SIZE;
  instr_queue_size--;
  return insn;
}

// Insert instruction into queue
void instr_queue_enqueue(instruction_t* insn) {
  assert(insn != NULL && instr_queue_size < INSTR_QUEUE_SIZE);
  int tail = (instr_queue_head + instr_queue_size) % INSTR_QUEUE_SIZE;
  instr_queue[tail] = insn;
  instr_queue_size++;
}

/* E552 Assignment 4 - END CODE */

//reservation stations (each reservation station entry contains a pointer to an instruction)
static instruction_t* reservINT[RESERV_INT_SIZE];
static instruction_t* reservFP[RESERV_FP_SIZE];

//functional units
static instruction_t* fuINT[FU_INT_SIZE];
static instruction_t* fuFP[FU_FP_SIZE];

//common data bus
static instruction_t* commonDataBus = NULL;

//The map table keeps track of which instruction produces the value for each register
static instruction_t* map_table[MD_TOTAL_REGS];

/* E552 Assignment 4 - BEGIN CODE */
// Print contents of map table
void print_map_table(int current_cycle) {
  printf("********** MAP TABLE CYCLE %d **********\n", current_cycle);
  int i;
  for(i = 0; i < MD_TOTAL_REGS; i++) {
    if(!map_table[i]) continue;
    printf("%d: ", i);
    PRINT_INST(stdout, map_table[i], "", current_cycle);
  }
  printf("\n");
}

/* E552 Assignment 4 - END CODE */

//the index of the last instruction fetched
static int fetch_index = 0;

/* FUNCTIONAL UNITS */


/* RESERVATION STATIONS */


/*
 * Description:
 * 	Checks if simulation is done by finishing the very last instruction
 *      Remember that simulation is done only if the entire pipeline is empty
 * Inputs:
 * 	sim_insn: the total number of instructions simulated
 * Returns:
 * 	True: if simulation is finished
 */
static bool is_simulation_done(counter_t sim_insn) {

  /* ECE552: YOUR CODE GOES HERE */
  return fetch_index > sim_insn
         && insn_array_is_empty(reservINT, RESERV_INT_SIZE)
         && insn_array_is_empty(reservFP, RESERV_FP_SIZE)
         && insn_array_is_empty(fuINT, FU_INT_SIZE)
         && insn_array_is_empty(fuFP, FU_FP_SIZE)
         && !commonDataBus;
}

/*
 * Description:
 * 	Retires the instruction from writing to the Common Data Bus
 * Inputs:
 * 	current_cycle: the cycle we are at
 * Returns:
 * 	None
 */
void CDB_To_retire(int current_cycle) {

  /* ECE552: YOUR CODE GOES HERE */
  if(!commonDataBus) return;

  // Clear dependencies from reservation stations
  {
    int i;
    for(i = 0; i < RESERV_INT_SIZE; i++) {
      instruction_t* insn = reservINT[i];
      if(!insn) continue;
      int j;
      // Remove dependencies in Q
      for(j = 0; j < MAX_INPUT_REGS; j++) {
        insn_array_remove_insn(commonDataBus, insn->Q, MAX_INPUT_REGS);
      }
    }
  }

  {
    int i;
    for(i = 0; i < RESERV_FP_SIZE; i++) {
      instruction_t* insn = reservFP[i];
      if(!insn) continue;
      int j;
      // Remove dependencies in Q
      for(j = 0; j < MAX_INPUT_REGS; j++) {
        insn_array_remove_insn(commonDataBus, insn->Q, MAX_INPUT_REGS);
      }
    }
  }

  // Remove mapping from map table if it's there
  {
    int i;
    for(i = 0; i < MAX_OUTPUT_REGS; i++) {
      insn_array_remove_insn(commonDataBus, map_table, MD_TOTAL_REGS);
    }
  }

  commonDataBus = NULL;
}


/*
 * Description:
 * 	Moves an instruction from the execution stage to common data bus (if possible)
 * Inputs:
 * 	current_cycle: the cycle we are at
 * Returns:
 * 	None
 */
void execute_To_CDB(int current_cycle) {

  /* ECE552: YOUR CODE GOES HERE */
  if(commonDataBus) return;

  // Get the oldest instruction done execution out of either the fuINT or fuFP
  instruction_t* oldest[2];
  oldest[0] =
    insn_array_get_oldest_ready_for_CDB(fuINT, FU_INT_SIZE, current_cycle);
  oldest[1] =
    insn_array_get_oldest_ready_for_CDB(fuFP, FU_FP_SIZE, current_cycle);
  instruction_t* insn =
    insn_array_get_oldest(oldest, 2);

  // Check if there's even any instructions ready
  if(!insn) return;

  // Put the insn that's ready on the CDB
  insn->tom_cdb_cycle = current_cycle;
  insn_array_remove_insn(insn, fuINT, FU_INT_SIZE);
  insn_array_remove_insn(insn, fuFP, FU_FP_SIZE);
  commonDataBus = insn;
}

/* E552 Assignment 4 - BEGIN CODE */
void issue_To_execute_helper(
  int current_cycle,
  instruction_t** reserv,
  int reserv_size,
  instruction_t** fu,
  int fu_size
) {
  // Check instructions in the reservation station and move them to the
  // functional units if all conditions met:
  // 1. All of their inputs are available
  // 2. There are functional units available
  while(!insn_array_is_empty(reserv, reserv_size)
        && !insn_array_is_full(fu, fu_size)) {
    // Get oldest ready instruction
    instruction_t* insn =
      insn_array_get_oldest_ready_for_execute(reserv, reserv_size);

    // If there's no oldest ready insn, we can't move anything to execute
    if(!insn) break;

    // Move insn from reservation station to functional units
    insn_array_remove_insn(insn, reserv, reserv_size);
    insn_array_insert_insn(insn, fu, fu_size);
    insn->tom_execute_cycle = current_cycle;
    //printf("********** MOVED INSN TO EXECUTE **********\n");
    //PRINT_INST(stdout, insn, "", current_cycle);
    //printf("pc %d\n", insn->pc);
  }
}
/* E552 Assignment 4 - END CODE */

/*
 * Description:
 * 	Moves instruction(s) from the issue to the execute stage (if possible). We prioritize old instructions
 *      (in program order) over new ones, if they both contend for the same functional unit.
 *      All RAW dependences need to have been resolved with stalls before an instruction enters execute.
 * Inputs:
 * 	current_cycle: the cycle we are at
 * Returns:
 * 	None
 */
void issue_To_execute(int current_cycle) {

  /* ECE552: YOUR CODE GOES HERE */
  issue_To_execute_helper(
    current_cycle,
    reservINT,
    RESERV_INT_SIZE,
    fuINT,
    FU_INT_SIZE
  );
  issue_To_execute_helper(
    current_cycle,
    reservFP,
    RESERV_FP_SIZE,
    fuFP,
    FU_FP_SIZE
  );
}

/*
 * Description:
 * 	Moves instruction(s) from the dispatch stage to the issue stage
 * Inputs:
 * 	current_cycle: the cycle we are at
 * Returns:
 * 	None
 */
void dispatch_To_issue(int current_cycle) {

  /* ECE552: YOUR CODE GOES HERE */
  // Get an instruction
  if(instr_queue_is_empty()) return;
  instruction_t* insn = instr_queue_peek();
  if(!insn) return;
  enum md_opcode op = insn->op;

  // Special case
  // Unconditional and conditional branches are handled the same way to model
  // a perfect branch predictor. Just update their dispatch cycles.
  if(IS_UNCOND_CTRL(op) || IS_COND_CTRL(op)) {
    insn = instr_queue_dequeue();
    return;
  }

  // Determine what kind of reservation station to use
  instruction_t** reserv;
  int size;
  if(USES_INT_FU(op)) {
    reserv = reservINT;
    size = RESERV_INT_SIZE;
  } else if(USES_FP_FU(op)) {
    reserv = reservFP;
    size = RESERV_FP_SIZE;
  } else {
    // Shouldn't really get here
    return;
  }

  // Check for free spots in the reservation station
  if(insn_array_is_full(reserv, size)) return;

  // We found a free spot in the reservation station!
  insn = instr_queue_dequeue();
  assert(insn);
  insn_array_insert_insn(insn, reserv, size);
  insn->tom_issue_cycle = current_cycle;

  // Check if we have any dependencies and update Q accordingly
  int r_in_idx;
  for(r_in_idx = 0; r_in_idx < MAX_INPUT_REGS; r_in_idx++) {
    int r_in = insn->r_in[r_in_idx];
    if(r_in < 0) continue;
    // Check if an instruction is writing to my input and update Q
    if(map_table[r_in]) {
      insn->Q[r_in_idx] = map_table[r_in];
    }
  }
  //print_insn_dependencies(insn, current_cycle);

  // Update the map table and tell it which registers I'm writing to
  int r_out_idx;
  for(r_out_idx = 0; r_out_idx < MAX_OUTPUT_REGS; r_out_idx++) {
    int r_out = insn->r_out[r_out_idx];
    if(r_out < 0) continue;
    map_table[r_out] = insn;
  }
  //print_map_table(current_cycle);
}

/*
 * Description:
 * 	Grabs an instruction from the instruction trace (if possible)
 * Inputs:
 *      trace: instruction trace with all the instructions executed
 * Returns:
 * 	None
 */
void fetch(instruction_trace_t* trace) {

  /* ECE552: YOUR CODE GOES HERE */

  // Check if the instruction queue is full or if there are any more insns
  fetch_index++;
  if(instr_queue_is_full() || fetch_index >= INSTR_TRACE_SIZE) {
    return;
  }

  // Get an instruction
  instruction_t* insn = get_instr(trace, fetch_index);
  if(!insn) return;

  // We don't want trap instructions
  if(IS_TRAP(insn->op)) {
    return;
  }

  // Insert the instruction in the queue
  instr_queue_enqueue(insn);
}

/*
 * Description:
 * 	Calls fetch and dispatches an instruction at the same cycle (if possible)
 * Inputs:
 *      trace: instruction trace with all the instructions executed
 * 	current_cycle: the cycle we are at
 * Returns:
 * 	None
 */
void fetch_To_dispatch(instruction_trace_t* trace, int current_cycle) {

  fetch(trace);

  /* ECE552: YOUR CODE GOES HERE */
  instruction_t* insn = instr_queue_peek();
  if(!insn) return;
  insn->tom_dispatch_cycle = current_cycle;
}

/*
 * Description:
 * 	Performs a cycle-by-cycle simulation of the 4-stage pipeline
 * Inputs:
 *      trace: instruction trace with all the instructions executed
 * Returns:
 * 	The total number of cycles it takes to execute the instructions.
 * Extra Notes:
 * 	sim_num_insn: the number of instructions in the trace
 */
counter_t runTomasulo(instruction_trace_t* trace)
{
  //initialize instruction queue
  int i;
  for (i = 0; i < INSTR_QUEUE_SIZE; i++) {
    instr_queue[i] = NULL;
  }

  //initialize reservation stations
  for (i = 0; i < RESERV_INT_SIZE; i++) {
      reservINT[i] = NULL;
  }

  for(i = 0; i < RESERV_FP_SIZE; i++) {
      reservFP[i] = NULL;
  }

  //initialize functional units
  for (i = 0; i < FU_INT_SIZE; i++) {
    fuINT[i] = NULL;
  }

  for (i = 0; i < FU_FP_SIZE; i++) {
    fuFP[i] = NULL;
  }

  //initialize map_table to no producers
  int reg;
  for (reg = 0; reg < MD_TOTAL_REGS; reg++) {
    map_table[reg] = NULL;
  }

  int cycle = 1;
  while (true) {

     /* ECE552: YOUR CODE GOES HERE */
     CDB_To_retire(cycle);
     execute_To_CDB(cycle);
     issue_To_execute(cycle);
     dispatch_To_issue(cycle);
     fetch_To_dispatch(trace, cycle);

     cycle++;

     if (is_simulation_done(sim_num_insn))
        break;
  }

  // Print some instructions for debugging purposes
  print_all_instr(trace, trace->size);

  return cycle;
}
