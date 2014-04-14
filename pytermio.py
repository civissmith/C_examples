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
  new_tty[3] &= ~termios.ICANON & ~termios.ECHO
  
  # Set the terminal attributes.
  termios.tcsetattr(fd, termios.TCSANOW, new_tty )

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
    for each in c:
      # Catch the escape character
      if each == '\x1B':
        print "INPUT: ^"
        continue
      # Catch the backspace character
      if each == '\x7F':
        print "INPUT: <BS>"
        continue
      print "INPUT: ", each

  # Set the terminal back to normal.
  termios.tcsetattr(fd, termios.TCSAFLUSH, saved_tty)
#}
#
#

#
# Invocation Check:
#
if __name__ == "__main__":
  main()
