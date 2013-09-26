# .cshrc - the shell reads this file at start-up, before .login

#set path = ( /local/packagers/cs161/bin $path)

# Environment:
# Set everything at the first invocation of the shell (which is not
# necessarily a login shell, e.g. when starting xterm from an rsh, so we
# can't do this in the .login).
if ( ! $?LEVEL ) then
	setenv LEVEL 1

	# Use umask 077 to prevent anyone from reading or writing your files.
	#
	umask 022

	# Set the shell's command search path.
	# This is a bit complicated and prone to change,
	# so we isolate the details in /slocal/etc/site.cshrc.
	# Some defaults may be overridden by setting certain environment
	# variables here.  For example, if you prefer Sun's version of
	# X windows (Open Windows), do this:
	#  setenv OPENWINHOME /usr/openwin
	# See /etc/site.cshrc for other possibilities.
	if ( -r /etc/site.cshrc ) then
		source /etc/site.cshrc
		set path=($path .)
	else
		# If there isn't any site specific source file,
		# this is a reasonable default.
		set path=(~/bin /local/bin /local/X11/bin /local/bin/X11 \
		/usr/bin/X11 /usr/ucb /bin /usr/bin /usr/ccs/bin .)
	endif

	# Here's how to add a directory to your shell's path.
	# MH users will want to uncomment this:
	#set path=($path /local/bin/mh)
	set path = ( $path /cad2/ece552f/compiler/bin )


	#ece334F Micromagic setup
	if( ! $?ECE334 ) then
	source /cad2/mmi/SOURCEME
	setenv ECE334 1
	endif

	# Put your own additional "setenv" commands here.
	#
	setenv EDITOR vi
	#setenv VISUAL vi
	setenv PAGER less
	setenv LESS "MeQd"
	setenv TERM xterm

	# The file /etc/printcap lists available printers.
	# Here's how to change the default printer:
	#lj251w (a.k.a. ljD) (HP LaserJet in GB251w)
	#lj251e (a.k.a. ljG) (HP LaserJet in GB251e)
	#lj243a (first HP LaserJet in GB243)
	#lj243b (other HP LaserJet in GB243)
	#lj144a (first HP LaserJet in GB144)
	#lj144b (other HP LaserJet in GB144)

	setenv PRINTER lj251w

	# To tell TeX to search your private macro library in ~/tex, uncomment
	# this.  The trailing colon is shorthand for the default TEXINPUTS value.
	#setenv TEXINPUTS .:$HOME/tex:

else
	# count the depth of nesting of the interactive shell.
	set level = $LEVEL; @ level++ ; setenv LEVEL $level
endif


# Interactive shell variables.
# Code inside this "if" statement is executed only for interactive sessions.
#
if ( $?prompt ) then
	unset autologout	# No autologout.
	#set ignoreeof		# Don't log out on Control-D.

	set history=250 notify filec
	set mail = ( 300 "/var/mail/$USER" )

	set lcd = ( )  #  add parents of frequently used directories

	# Sample code to handle differences between tcsh and csh.
	# if ( $?tcsh ) then
	#	bindkey -v	# use this to switch to vi-style key bindings
	# endif

	# If you don't like the default terminal settings,
	# use the "stty" command to change them here,
	# for example, if you want ^H to backspace and DELETE to interrupt,
	# use this:
	stty erase '^H' intr '^?'

	# Disable core dumps from aborting programs.
	# If you are doing program development, comment out this line.
	limit coredumpsize 0
endif

# Interactive shell aliases.
#
alias rm	/bin/rm -i

# NOTE!
# Do not change your $path (or add any "source .." lines) here!
#  Make $path changes above, after the comment line which says,
#  "Here's how to add a directory to your shell's path."
