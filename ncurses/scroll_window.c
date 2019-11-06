#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

static int mrow, mcol;

void quit(void) {
    endwin();
}

int main(int argc, char **argv)
{
    // initialize curses
    initscr();
    atexit(quit);
    clear();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();

    // get terminal size
    getmaxyx(stdscr, mrow, mcol);

    curs_set(0); // Cursor off

    // create pad
    int rowcount = 100;
    initscr();
    int height, width;
    getmaxyx(stdscr, height, width);
    //WINDOW *pad = newpad (rowcount + 1, mcol);
    WINDOW* pad = newwin(height - 2, width - 2, 1, 1);
    scrollok(pad, TRUE);

    // col titles
    wprintw(pad, "Auftrag \tName \t\t\tZeit\n");

    for (unsigned i=0; i<10000; i++) {
        wprintw(pad, "%d - lots and lots of lines flowing down the terminal\n", i);
        wrefresh(pad);
    }

    // Show content of pad
    int mypadpos = 0;
    //prefresh(pad, mypadpos, 0, 0, 0, mrow, mcol);

    // wait for exit key
    int ch;
    while ((ch = wgetch(pad)) != 'q')
    {
        switch (ch)
        {
        case KEY_UP:
            if (mypadpos >= 0)
            {
                mypadpos--;
            }
            prefresh(pad, mypadpos, 0, 0, 0, mrow, mcol);
            break;
        case KEY_DOWN:
            if (mypadpos <= rowcount+1)
            {
                mypadpos++;
            }
            prefresh(pad, mypadpos, 0, 0, 0, mrow, mcol);
            break;
        }
    }

    // remove window
    delwin(pad);
    clear();
    refresh();

    return (0);
}

