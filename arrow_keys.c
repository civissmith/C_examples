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
#include <string.h>

static struct termios save_termios;
#define MINBYTES 3
#define TIME     1

int main(){

  struct termios buffer;
  int i;
  int j;
  char run;
  char c[MINBYTES];

  /* Save the current terminal setting so they can be restored on exit. */
  if( tcgetattr(STDIN_FILENO, &save_termios) < 0) {
    printf("Could not save terminal...\n");
    exit(1);
  }

  /* Set the terminal to no ECHO and disable canonical mode */
  buffer.c_lflag &= ~(ECHO | ICANON );

  /* Set input to read MINBYTE byte at a time, TIME tenths of a second timer */
  buffer.c_cc[VMIN] = MINBYTES;
  buffer.c_cc[VTIME] = TIME;
  if( tcsetattr(STDIN_FILENO, TCSAFLUSH, &buffer) < 0){
    printf("Could not set terminal correctly.\n");
    exit(2);
  }
  if ( (buffer.c_lflag & (ECHO | ICANON )) || buffer.c_cc[VMIN] != MINBYTES || buffer.c_cc[VTIME] != TIME){
    printf("Terminal not set correctly.\n");
    exit(3);
  
  }
  
  /* Read from the terminal */
  run = 1;
  printf("\e[2J\e[;HPress BACKSPACE to quit...\n");
  while( run ){
    /* Clear the buffer each time */
    memset( c, '\0', MINBYTES );
    i = read(STDIN_FILENO, &c, MINBYTES);
    if( i == -1 ){
       printf("Read error!\n");
       break;
    }

    /* Catch the UP arrow */
    if( !(strcmp( c, "\e[A") )){
      printf("\e[2J");
      printf("\e[5;20HHey, an UP ARROW");
      fflush(stdout);
      continue;
    } 
    /* Catch the DOWN arrow */
    if( !(strcmp( c, "\e[B") )){
      printf("\e[2J");
      printf("\e[15;20HHey, an DOWN ARROW");
      fflush(stdout);
      continue;
    } 
    /* Catch the RIGHT arrow */
    if( !(strcmp( c, "\e[C") )){
      printf("\e[2J");
      printf("\e[10;35HHey, an RIGHT ARROW");
      fflush(stdout);
      continue;
    } 
    /* Catch the LEFT arrow */
    if( !(strcmp( c, "\e[D") )){
      printf("\e[2J");
      printf("\e[10;5HHey, an LEFT ARROW");
      fflush(stdout);
      continue;
    } 


    printf("\e[2;1H");
    for( j = 0; j < MINBYTES; j++ ){
      if( (c[j] &= 255 ) == 0177){
        run = 0;
        break;
      }
      /* Set the cursor before writing characters */
      if( c[j] != '\0' ){
         printf("0x%0x:%c ", c[j], c[j]);
      }else{
         printf("\e[K");
      }
      fflush(stdout);
   
    }
  }

  if( tcsetattr(STDIN_FILENO, TCSAFLUSH, &save_termios) < 0){
    printf("Could not set terminal correctly.\n");
    exit(2);
  }

  return EXIT_SUCCESS;
}
