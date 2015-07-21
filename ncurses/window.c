/*
  CURWIN1.C
  =========
  (c) Copyright Paul Griffiths 1999
  Email: mail@paulgriffiths.net

  Moving windows with ncurses.
*/


#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

int main(void) {

    /*  Set the dimensions and initial position for our child window   */
    int      width = 30, height = 10;
    int      rows  = 25, cols   = 80;
    int      x = (cols - width)  / 2;
    int      y = (rows - height) / 2;

    WINDOW* mainwin = initscr();
    if (mainwin == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }


    /*  Switch of echoing and enable keypad (for arrow keys)  */
    noecho();
    keypad(mainwin, TRUE);

    /*  Make our child window, and add
    a border and some text to it.   */
    int res = 0;

    WINDOW* childwin = subwin(mainwin, height, width, y, x);
    res = box(childwin, 0, 0);
    if (res != OK) {
        fprintf(stderr, "error box()\n");
        exit(-1);
    }
    mvwaddstr(childwin, 1, 4, "Move the window");
    mvwaddstr(childwin, 2, 2, "with the arrow keys");
    mvwaddstr(childwin, 3, 6, "or HOME/END");
    mvwaddstr(childwin, 5, 3, "Press 'q' to quit");

    refresh();

    int ch = 0;
    while ( (ch = getch()) != 'q' ) {
        switch ( ch ) {
        case KEY_UP:
            if ( y > 0 ) --y;
            break;
        case KEY_DOWN:
            if ( y < (rows - height) ) ++y;
            break;
        case KEY_LEFT:
            if ( x > 0 ) --x;
            break;
        case KEY_RIGHT:
            if ( x < (cols - width) ) ++x;
            break;
        case KEY_HOME:
            x = 0;
            y = 0;
            break;
        case KEY_END:
            x = (cols - width);
            y = (rows - height);
            break;
        }

        if (ERR == mvwin(childwin, y, x)) {
            fprintf(stderr, "ERROR mvwin()\n");
            exit(-1);
        }
        mvwprintw(childwin, 7, 2, "You pressed: 0x%0x", ch);
        wrefresh(childwin);
        // hmm still leaves shadow..
    }


    delwin(childwin);
    delwin(mainwin);
    endwin();
    refresh();
    return EXIT_SUCCESS;
}



