/*******************************************************************************
* @Title: winsize.c
*
* @Author: Phil Smith
*
* @Date: Thu, 10-Apr-14 05:21PM
*
* @Project: APUE 2e Example Code
*
* @Purpose: Terminal I/O
*
* @Revision:
* $Id: $
*
*******************************************************************************/
#include <termios.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif
#include <stdlib.h>

/**
 * pr_winsize: prints the window size.
 **/
static void pr_winsize(int fd){

  struct winsize size;
  if( ioctl(fd, TIOCGWINSZ, (char*) &size) < 0){
     printf("Bad\n");
     exit(1);
  }
  printf("%d rows, %d columns\n", size.ws_row, size.ws_col);

}

/**
 * sig_winch: Signal handler for window-change signal.
 **/
static void sig_winch(int signo){
  printf("SIGWINCH received\n");
  pr_winsize(STDIN_FILENO);
}


int main()
{
  if( isatty(STDIN_FILENO) == 0)
    exit(2);

  /* Note: signal() is deprecated, sigaction() should be used in production
   *       code. */
  if(signal(SIGWINCH, sig_winch) == SIG_ERR){
    printf("Signal error!\n");
    exit(3);
  }

  /* Print the size initially */
  pr_winsize(STDIN_FILENO);
    while(1)
      pause();
}
