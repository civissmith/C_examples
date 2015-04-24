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
#include <stdlib.h>
#define TMPSTRSZ  20
#define RED    41
#define GREEN  42
#define YELLOW 43
#define BLUE   44
#define CYAN   46
#define WHITE  47

/* clear_screen: clears the screen and re-homes the cursor */
void clear_screen(  )
{
    printf( "\E[2J\E[;H" );
}

/* set_cursor_pos: sets the cursor to the given x and y pos */
void set_cursor_pos( int row, int col )
{
    char tmpStr[TMPSTRSZ] = { '\0' };
    sprintf( tmpStr, "\E[%d;%dH", row, col );
    printf( "%s", tmpStr );
}

/* highlight: highlights a string */
char *highlight( const char *src, char *dest, int color )
{
    sprintf( dest, "\E[%dm%s\E[0m", color, src );
    return dest;
}

int main(  )
{
    int i = 10;
    char tmpStr[TMPSTRSZ];
    clear_screen(  );
    while ( i-- > 0 ) {
        set_cursor_pos( 1, 17 );
        if ( i < 3 ) {
            printf( "%s", highlight( "Hello World!\n", tmpStr, CYAN ) );
        } else {
            printf( "%s", "Hello World!\n" );
        }
        set_cursor_pos( 2, 17 );
        if ( i < 6 && i >= 3 ) {
            printf( "%s", highlight( "Salud Monde!\n", tmpStr, CYAN ) );
        } else {
            printf( "%s", "Salud Monde!\n" );
        }
        set_cursor_pos( 3, 17 );
        if ( i < 9 && i >= 6 ) {
            printf( "%s", highlight( "Goodbye World!\n", tmpStr, CYAN ) );
        } else {
            printf( "%s", "Goodbye World!\n" );
        }
        sleep( 1 );
    }

    return EXIT_SUCCESS;
}
