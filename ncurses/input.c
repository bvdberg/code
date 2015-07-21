/*
  CURIN1.C
  ========
  (c) Copyright Paul Griffiths 1999
  Email: mail@paulgriffiths.net

  Demonstrating basic ncurses single key input.
*/

#include <stdlib.h>
#include <ctype.h>
#include <curses.h>

struct keydesc {
    int  code;
    char name[20];
};

static const char* intprtkey(int ch) {
    static struct keydesc keys[] = { { KEY_UP,        "Up arrow"        },
        { KEY_DOWN,      "Down arrow"      },
        { KEY_LEFT,      "Left arrow"      },
        { KEY_RIGHT,     "Right arrow"     },
        { KEY_HOME,      "Home"            },
        { KEY_END,       "End"             },
        { KEY_BACKSPACE, "Backspace"       },
        { KEY_IC,        "Insert"          },
        { KEY_DC,        "Delete"          },
        { KEY_NPAGE,     "Page down"       },
        { KEY_PPAGE,     "Page up"         },
        { KEY_F(1),      "Function key 1"  },
        { KEY_F(2),      "Function key 2"  },
        { KEY_F(3),      "Function key 3"  },
        { KEY_F(4),      "Function key 4"  },
        { KEY_F(5),      "Function key 5"  },
        { KEY_F(6),      "Function key 6"  },
        { KEY_F(7),      "Function key 7"  },
        { KEY_F(8),      "Function key 8"  },
        { KEY_F(9),      "Function key 9"  },
        { KEY_F(10),     "Function key 10" },
        { KEY_F(11),     "Function key 11" },
        { KEY_F(12),     "Function key 12" },
        { -1,            "<unsupported>"   }
    };
    static char keych[2] = {0};

    if ( isprint(ch) && !(ch & KEY_CODE_YES)) {
        keych[0] = ch;
        return keych;
    } else {

        /*  Non-printable, so loop through our array of structs  */
        int n = 0;
        do {
            if ( keys[n].code == ch )
                return keys[n].name;
            n++;
        } while ( keys[n].code != -1 );

        return keys[n].name;
    }
    return NULL;        /*  We shouldn't get here  */
}

int main(void) {
    int ch;

    WINDOW * mainwin = initscr();;

    if (mainwin == NULL) {
        fprintf(stderr, "Error initializing ncurses.\n");
        exit(EXIT_FAILURE);
    }

    noecho();                  /*  Turn off key echoing                 */
    keypad(mainwin, TRUE);     /*  Enable the keypad for non-char keys  */

    /*  Print a prompt and refresh() the screen  */

    mvaddstr(5, 10, "Press a key ('q' to quit)...");
    mvprintw(7, 10, "You pressed: ");
    refresh();

    /*  Loop until user presses 'q'  */

    while ( (ch = getch()) != 'q' ) {

        /*  Delete the old response line, and print a new one  */

        deleteln();
        mvprintw(7, 10, "You pressed: 0x%x (%s)", ch, intprtkey(ch));
        refresh();
    }

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}

