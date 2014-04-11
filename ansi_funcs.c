/*******************************************************************************
* @Title: ansi_funcs.c
*
* @Author: Phil Smith
*
* @Date: Fri, 11-Apr-14 02:55AM
*
* @Project: 
*
* @Purpose:
*
* @Revision:
* $Id: $
*
*******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#define TMPSTRSZ  20

/* clear_screen: clears the screen and re-homes the cursor */
void clear_screen(){
  printf("\E[2J\E[;H");
}
/* set_cursor_pos: sets the cursor to the given x and y pos */
void set_cursor_pos( int x, int y ){
  char tmpStr[TMPSTRSZ] = {'\0'};
  sprintf(tmpStr, "\E[%d;%dH", x, y);
  printf( "%s",tmpStr );
}

int main(){
  int i = 10;
  clear_screen();
  while( i-- > 0 ){
     set_cursor_pos( 4, 4 );
     printf("Hello World!\n");
     set_cursor_pos( 10, 17 );
     printf("Goodbye World!\n");
     sleep(1);
  }
  
}
