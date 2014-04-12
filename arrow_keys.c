/*******************************************************************************
* @Title: arrow_keys.c
*
* @Author: Phil Smith
*
* @Date: Sat, 12-Apr-14 08:33AM
*
* @Project: Terminal I/O
*
* @Purpose: Set the terminal to non-canonical mode and intercept arrow keys.
*
* @Revision:
* $Id: $
*
*******************************************************************************/
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static struct termios save_termios;
static int saved_tty_fd;
static enum { 
   RESET,
   RAW,
   CBREAK
} ttyState;

int main(){

  struct termios buffer;
  int i;
  char c;

  /* Save the current terminal setting so they can be restored on exit. */
  if( tcgetattr(STDIN_FILENO, &save_termios) < 0) {
    printf("Could not save terminal...\n");
    exit(1);
  }

  /* Set the terminal to no ECHO and disable canonical mode */
  buffer.c_lflag &= ~(ECHO | ICANON );

  /* Set input to read 1 byte at a time, no timer */
  buffer.c_cc[VMIN] = 1;
  buffer.c_cc[VTIME] = 0;
  if( tcsetattr(STDIN_FILENO, TCSAFLUSH, &buffer) < 0){
    printf("Could not set terminal correctly.\n");
    exit(2);
  }
  if ( (buffer.c_lflag & (ECHO | ICANON )) || buffer.c_cc[VMIN] != 1 || buffer.c_cc[VTIME] != 0){
    printf("Terminal not set correctly.\n");
    exit(3);
  
  }
  
  /* Read from the terminal */
  while( (i = read(STDIN_FILENO, &c, 1)) == 1){
    if( (c &= 255 ) == 0177)
      break;
    printf("%o\n", c);
  }

  if( tcsetattr(STDIN_FILENO, TCSAFLUSH, &save_termios) < 0){
    printf("Could not set terminal correctly.\n");
    exit(2);
  }
}
