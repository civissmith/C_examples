#!/usr/bin/python -B
################################################################################
# @Title: menu.py
#
# @Author: Phil Smith
#
# @Date: Sun, 13-Apr-14 08:21PM
#
# @Project: Python Terminal I/O
#
# @Purpose: Draw a simple arrow selectable menu.
#
# @Revision:
# $Id: $
#
################################################################################
import termios
import sys
import os
import tty

#
# main(): Main entry point for this program
#{
def main():
  selections = [
  "1. Selection 1",
  "2. Selection 2",
  "3. Selection 3",
  "4. Selection 4",
  "5. Selection 5",
  ]

  # Use stdin as the file descriptor
  fd = sys.stdin.fileno()

  #
  # Save the terminal attributes
  #
  saved_tty = termios.tcgetattr(fd)

  #
  # Get a local copy of the attributes to modify
  #
  local_tty = termios.tcgetattr(fd)

  #
  # Modify the terminal attributes and set the flags
  #
  local_tty[3] &= ~termios.ECHO
  local_tty[3] &= ~termios.ICANON
  termios.tcsetattr(fd, termios.TCSANOW, local_tty)
  
  #
  # The terminal is configured, loop until told to quit
  #
  cmd = 'n'
  selected = 0
  length = len(selections) - 1
  try:
    while cmd not in 'Qq':
      
      move_cursor(5,1)
      print "Please make a selection:"
      print "Enter 'Q' or 'q' to exit"
      for i in range(0, len(selections)): 
        if i == selected:
          print_line( selections[i], 'YELLOW', 'RED' )
        else:
          print_line( selections[i] )

      try:
        # Try to get 3 bytes to handle arrow keys
        cmd = os.read(fd, 3)
      except Exception as e:
        print "Unhandled exception reading commands"
        print type(e)
        break

      #
      # Handle input commands
      #

      # UP ARROW
      if cmd == '\x1B[A':
         selected -= 1
      # DOWN ARROW
      if cmd == '\x1B[B':
         selected += 1
      # RIGHT ARROW
      if cmd == '\x1B[C':
         selected -= 1
      # LEFT ARROW
      if cmd == '\x1B[D':
         selected += 1
      move_cursor()

      # Range check the highlight
      if selected > length:
         selected = 0
      if selected < 0:
         selected = length

  finally:
    # On the way out, set the attributes back
    termios.tcsetattr(fd, termios.TCSANOW, saved_tty)
#}
# end main()
#

#
# test_print_line(): Tests the output from the print_line function
#{                  
def test_print_line():
  selections = [
  "1. Selection 1",
  "2. Selection 2",
  "3. Selection 3",
  "4. Selection 4",
  "5. Selection 5",
  ]
  for selection in selections:
    print_line( selection, "YELLOW", "GREEN" )
    print_line( selection, "RED",    "BLUE" )
    print_line( selection, "GREEN",  "RED" )
    print_line( selection, bg="YELLOW")
    print_line( selection, bg="RED")
    print_line( selection, bg="GREEN")
    print_line( selection, bg="BLUE")
    print_line( selection, fg="YELLOW")
    print_line( selection, fg="RED")
    print_line( selection, fg="GREEN")
    print_line( selection, fg="BLUE")
#}
# end test_print_line()()
#

#
# print_line( text, bg, fg ): Print a line of text. Optionally change the
#                             background (bg) or foreground (fg) color.
# Colors Supported:
# YELLOW
# RED
# GREEN
# BLUE
#{
def print_line( text, bg=None, fg=None):
  if not bg:
    bg = ''
  if not fg:
    fg = ''

  # Set the background colors
  if 'RED' in bg.upper():
    bg = '\x1B[41m'
  if 'GREEN' in bg.upper():
    bg = '\x1B[42m'
  if 'YELLOW' in bg.upper():
    bg = '\x1B[43m'
  if 'BLUE' in bg.upper():
    bg = '\x1B[44m'

  # Set the foreground colors
  if 'RED' in fg.upper():
    fg = '\x1B[31m'
  if 'GREEN' in fg.upper():
    fg = '\x1B[32m'
  if 'YELLOW' in fg.upper():
    fg = '\x1B[33m'
  if 'BLUE' in fg.upper():
    fg = '\x1B[34m'

 
  sys.stdout.write(bg + fg + text + '\x1B[0m\n')
#}
# end print_line()
#

#
# move_cursor(): Set the cursor to the location. Default is to
#                'home' the cursor.
#{
def move_cursor( row=1, col=1):
  sys.stdout.write('\x1B[%d;%dH'%(row, col))  
#}
#
#


#
# Invocation Check:
#
if __name__ == "__main__":
  main()
