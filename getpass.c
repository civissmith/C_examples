/*******************************************************************************
* @Title: getpass.c
*
* @Author: Phil Smith
*
* @Date: Wed, 09-Apr-14 09:38PM
*
* @Project: APUE 2e
*
* @Purpose: Terminal I/O
*
* @Revision:
* $Id: $
*
*******************************************************************************/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

/* Max num of chars for user to enter */
#define MAX_PASS_LEN 8

char *getpass( const char *prompt )
{

    static char buf[MAX_PASS_LEN + 1];
    char *ptr;
    sigset_t sig, osig;
    struct termios ts, ots;
    FILE *fp;
    int c;

    if ( ( fp = fopen( ctermid( NULL ), "r+" ) ) == NULL ) {
        return ( NULL );
    }
    setbuf( fp, NULL );


    sigemptyset( &sig );

    /* Block SIGINT and SIGTSTP */
    sigaddset( &sig, SIGINT );
    sigaddset( &sig, SIGTSTP );
    sigprocmask( SIG_BLOCK, &sig, &osig );

    tcgetattr( fileno( fp ), &ts );
    ots = ts;
    ts.c_lflag &= ~( ECHO | ECHOE | ECHOK | ECHONL );
    tcsetattr( fileno( fp ), TCSAFLUSH, &ts );

    fputs( prompt, fp );

    ptr = buf;
    while ( ( c = getc( fp ) ) != EOF && c != '\n' ) {
        if ( ptr < &buf[MAX_PASS_LEN] ) {
            *ptr++ = c;
        }
    }
    *ptr = 0;
    putc( '\n', fp );
    tcsetattr( fileno( fp ), TCSAFLUSH, &ots );
    sigprocmask( SIG_SETMASK, &osig, NULL );
    fclose( fp );
    return ( buf );
}

int main(  )
{
    char *ptr;
    if ( ( ptr = getpass( "Enter Password:" ) ) == NULL ) {
        printf( "Error with getpass()!\n" );
        exit( 1 );
    }
    printf( "\E[2J\E[;H" );
    printf( "password: \E[45m%s\E[0m\n", ptr );

    return EXIT_SUCCESS;
}
