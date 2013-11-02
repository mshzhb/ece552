
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

  return true; //ECE552: you can change this as needed; we've added this so the code provided to you compiles
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

}

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

  // Get an instruction
  if(instr_queue_is_empty()) return;
  instruction_t* insn = instr_queue_peek();
  enum md_opcode op = insn->op;

  // Special case
  // Unconditional and conditional branches are handled the same way to model
  // a perfect branch predictor. Just update their dispatch cycles.
  if(IS_UNCOND_CTRL(op) || IS_COND_CTRL(op)) {
    insn = instr_queue_dequeue();
    insn->tom_dispatch_cycle = current_cycle;
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
  }

  // Check for free spots in the reservation station
  int reserv_idx;
  for(reserv_idx = 0; reserv_idx < size; reserv_idx++) {
    // Check for empty (or NULL) spots
    if(!reserv[reserv_idx]) break;
  }
  // Nothing free, gotta stall
  if(reserv_idx == size) return;

  // We found a free spot in the reservation station!
  insn = instr_queue_dequeue();
  insn->tom_dispatch_cycle = current_cycle;
  reserv[reserv_idx] = insn;

  // Check if we have any dependencies
  int r_in_idx;
  for(r_in_idx = 0; r_in_idx < MAX_INPUT_REGS; r_in_idx++) {
    int r_in = insn->r_in[r_in_idx];
    if(r_in < 0) continue;
    // Check if an instruction is writing to my input
    if(map_table[r_in]) {
      insn->Q[r_in_idx] = map_table[r_in];
    }
  }
  print_insn_dependencies(insn, current_cycle);

  // Update the map table and tell it which registers I'm writing to
  int r_out_idx;
  for(r_out_idx = 0; r_out_idx < MAX_OUTPUT_REGS; r_out_idx++) {
    int r_out = insn->r_out[r_out_idx];
    map_table[r_out] = insn;
  }
  print_map_table(current_cycle);
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
  //while (true) {
  while (cycle <= 1000) {

     /* ECE552: YOUR CODE GOES HERE */
     fetch_To_dispatch(trace, cycle);

     cycle++;

    /*
     if (is_simulation_done(sim_num_insn))
        break;
    */
  }

  // Print some instructions for debugging purposes
  print_all_instr(trace, 20);

  return cycle;
}
