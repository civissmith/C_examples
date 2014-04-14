#!/usr/bin/python -B
################################################################################
# @Title: pytermio.py
#
# @Author: Phil Smith
#
# @Date: Sun, 13-Apr-14 04:53PM
#
# @Project: Terminal I/O
#
# @Purpose: Demonstrate terminal I/O in Python.
#
# @Revision:
# $Id: $
#
################################################################################
import termios
import sys
import os

#
# main: Main entry point
#{
def main():
  
  # Get STDIN file descriptor
  fd = sys.stdin.fileno()

  # Save the initial settings so that they can be restored.
  saved_tty = termios.tcgetattr(fd)

  # Get the current settings, they can be modified then set.
  new_tty = termios.tcgetattr(fd)

  # According to docs (https://docs.python.org/2/library/termios.html#termios.tcgetattr)
  # Structure of new_tty is:
  #   new_tty {
  # 0   iflag,
  # 1   oflag,
  # 2   cflag,
  # 3   lflag,
  # 4   ispeed,
  # 5   ospeed,
  # 6   cc[]         < Each element is a string of length 1 except VMIN and VTIME (integers)
  #   } 


  # Set the lflags to disable canonical mode and echoing
  new_tty[3] &= termios.ICANON & termios.ECHO
  
  # Set the terminal attributes.
  termios.tcsetattr(fd, termios.TCSANOW, new_tty )

  # Clear up the first line before drawing the message
  clear_screen()
  move_cursor()
  print "Press Q or q to quit"


  while 1:

    c = None
    #  Read a character, if anything goes wrong break out of the loop.
    try:
      c = os.read(fd, 3)
    except:
      break

    if c in "Qq":
       break
    
    # Look for arrows.
    if c == '\x1B[A':
       kill_line(5)
       kill_line(15)
       kill_line(10)
       move_cursor(5,20)
       print "Up Arrow"
       continue
    if c == '\x1B[B':
       kill_line(5)
       kill_line(15)
       kill_line(10)
       move_cursor(15,20)
       print "Down Arrow"
       continue
    if c == '\x1B[C':
       kill_line(5)
       kill_line(15)
       kill_line(10)
       move_cursor(10,35)
       print "Right Arrow"
       continue
    if c == '\x1B[D':
       kill_line(5)
       kill_line(15)
       kill_line(10)
       move_cursor(10,5)
       print "Left Arrow"
       continue

    move_cursor()
    kill_line()
    for each in c:
      # Catch the escape character
      if each == '\x1B':
        out = ('0x1B', '^')
      # Catch the backspace character
      elif each == '\x7F':
        out = ('0x7F', '<BS>')
      # Catch the newline character
      elif each == '\x0A':
        out = ('0x0A', '<NL>')
      else: 
        out = (hex(ord(each)), each)

      # Just in case I missed other conditions that could fail,
      # catch all exceptions and break. If the program dies here,
      # the terminal will not be restored to a usable state.
      try:
        print "%s: %s " % out
      except Exception as e:
        print "\nCaught exception: ", type(e)
        break

  # Clean up the arrow lines.
  kill_line(5)
  kill_line(10)
  kill_line(15)

  move_cursor()
  # Set the terminal back to normal.
  termios.tcsetattr(fd, termios.TCSAFLUSH, saved_tty)
#}
#
#

#
# move_cursor: Move the cursor to a specific location
#              default action is to "home" the cursor
#{
def move_cursor(row=1, col=1):
  # Use sys.stdout.write() - print() will not honor ANSI codes.
  sys.stdout.write( "\x1B[%d;%dH" % (row, col))
#}
#
#

#
# kill_line: Move the cursor to a specific location
#            and kills the line.
#{
def kill_line(row=1, col=1):
  # Use sys.stdout.write() - print() will not honor ANSI codes.
  move_cursor(row,col)
  sys.stdout.write( "\x1B[K" )
#}
#
#

#
# clear_screen: Issues the ANSI clear sequence. It clears the screen at the
#               cost of eating terminal history.
#{
def clear_screen():
  # Use sys.stdout.write() - print() will not honor ANSI codes.
  sys.stdout.write( "\x1B[2J" )
#}
#
#

#
# Invocation Check:
#
if __name__ == "__main__":
  main()
