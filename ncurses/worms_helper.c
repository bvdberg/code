/*

  HELPER.C
  ========
  (c) Copyright Paul Griffiths 2000
  Email: mail@paulgriffiths.net

  Helper functions.

*/


#include <stdlib.h>

#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>

#include "worms_helper.h"
#include "worms.h"


/*  Quit on error  */

void Error_Quit(char * msg) {

    extern WINDOW * mainwin;
    extern int oldcur;


    /*  Clean up nicely  */

    delwin(mainwin);
    curs_set(oldcur);
    endwin();
    refresh();
    FreeWorm();


    /*  Output error message and exit  */

    perror(msg);
    exit(EXIT_FAILURE);
}


/*  Quit successfully  */

void Quit(int reason) {

    extern WINDOW * mainwin;
    extern int oldcur;
    extern int score;


    /*  Clean up nicely  */

    delwin(mainwin);
    curs_set(oldcur);
    endwin();
    refresh();
    FreeWorm();


    /*  Output farewell message  */

    switch ( reason ) {
    case HITWALL:
        printf("\nYou hit a wall!\n");
        printf("Your score is %d\n", score);
        break;

    case HITSELF:
        printf("\nYou ran into yourself!\n");
        printf("Your score is %d\n", score);
        break;

    default:
        printf("\nGoodbye!\n");
        break;
    }

    exit(EXIT_SUCCESS);
}


/*  Returns the x-y size of the terminal  */

void GetTermSize(int * rows, int * cols) {

    struct winsize ws;


    /*  Get terminal size  */

    if ( ioctl(0, TIOCGWINSZ, &ws) < 0 ) {
        perror("couldn't get window size");
        exit(EXIT_FAILURE);
    }


    /*  Update globals  */

    *rows = ws.ws_row;
    *cols = ws.ws_col;
}


/*  Signal handler  */

void handler(int signum) {

    extern WINDOW * mainwin;
    extern int oldcur;


    /*  Switch on signal number  */
    switch ( signum ) {
    case SIGALRM:
        /*  Received from the timer  */
        MoveWorm();
        return;
    case SIGTERM:
    case SIGINT:
        /*  Clean up nicely  */
        delwin(mainwin);
        curs_set(oldcur);
        endwin();
        refresh();
        FreeWorm();
        exit(EXIT_SUCCESS);

    }
}

