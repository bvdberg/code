/*

  MAIN.C
  ======
  (c) Copyright Paul Griffiths 2000
  Email: mail@paulgriffiths.net

  Worms game using ncurses.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <signal.h>
#include <sys/time.h>
#include <curses.h>

#include "worms.h"
#include "worms_helper.h"


#define TIMESTEP 100000

void SetTimer(void);
void SetSignals(void);


WINDOW * mainwin;
int oldcur;


int main(void) {

    /*  Seed RNG, set timer and register signal handlers  */

    srand( (unsigned) time(NULL) );
    SetTimer();
    SetSignals();


    /*  Initialize ncurses  */

    if ( (mainwin = initscr()) == NULL ) {
        perror("error initialising ncurses");
        exit(EXIT_FAILURE);
    }

    noecho();
    keypad(mainwin, TRUE);
    oldcur = curs_set(0);


    /*  Initialize worm and draw it  */

    InitWorm();
    Draw();


    /*  Loop and get user input  */

    while ( 1 ) {
        int key = getch();

        switch ( key ) {

        case KEY_UP:
        case 'Y':
        case 'y':
            ChangeDir(UP);
            break;

        case KEY_DOWN:
        case 'N':
        case 'n':
            ChangeDir(DOWN);
            break;

        case KEY_LEFT:
        case 'G':
        case 'g':
            ChangeDir(LEFT);
            break;

        case KEY_RIGHT:
        case 'J':
        case 'j':
            ChangeDir(RIGHT);
            break;

        case 'Q':
        case 'q':
            Quit(USER_QUIT);
            break;

        }
    }


    /*  We never get here  */

    return EXIT_SUCCESS;
}


/*  Sets up the game timer  */

void SetTimer(void) {

    struct itimerval it;


    /*  Clear itimerval struct members  */

    timerclear(&it.it_interval);
    timerclear(&it.it_value);


    /*  Set timer  */

    it.it_interval.tv_usec = TIMESTEP;
    it.it_value.tv_usec    = TIMESTEP;
    setitimer(ITIMER_REAL, &it, NULL);
}


/*  Sets up signal handlers we need  */

void SetSignals(void) {

    struct sigaction sa;


    /*  Fill in sigaction struct  */

    sa.sa_handler = handler;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);


    /*  Set signal handlers  */

    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);


    /*  Ignore SIGTSTP  */

    sa.sa_handler = SIG_IGN;
    sigaction(SIGTSTP, &sa, NULL);
}

