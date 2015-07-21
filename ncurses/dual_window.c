/*
    - show lines to scroll through (like MC)
*/

#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>

static uint64_t getCurrentTime()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += now.tv_usec;
    return now64;
}

static void initColors() {
    if (has_colors() == FALSE) {
        fprintf(stderr, "no color support!\n");
        exit(-1);
    }
    start_color();
    init_pair(0,  COLOR_WHITE,   COLOR_BLACK);
    init_pair(1,  COLOR_RED,     COLOR_BLACK);
    init_pair(2,  COLOR_GREEN,   COLOR_BLACK);
    init_pair(3,  COLOR_YELLOW,  COLOR_BLACK);
    init_pair(4,  COLOR_BLUE,    COLOR_BLACK);
    init_pair(5,  COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6,  COLOR_CYAN,    COLOR_BLACK);
    init_pair(7,  COLOR_BLUE,    COLOR_WHITE);
    init_pair(8,  COLOR_WHITE,   COLOR_RED);
    init_pair(9,  COLOR_BLACK,   COLOR_GREEN);
    init_pair(10, COLOR_BLUE,    COLOR_YELLOW);
    init_pair(11, COLOR_WHITE,   COLOR_BLUE);
    init_pair(12, COLOR_WHITE,   COLOR_MAGENTA);
    init_pair(13, COLOR_BLACK,   COLOR_CYAN);
}

static WINDOW* create_window(WINDOW* parent, int h, int w, int y, int x, const char* title) {
    WINDOW* win = subwin(parent, h, w, y, x);
    box(win, 0, 0);
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, w-2);
    mvwaddch(win, 2, w-1, ACS_RTEE);
    //mvwaddstr(win, 5, 3, "Press 'q' to quit");
    int middle = (w - strlen(title)) / 2;
    wcolor_set(win, 5, NULL);
    mvwprintw(win, 1, middle, "%s", title);
    return win;
}

static char *choices[] = {
    "Choice 1",
    "Choice 2",
    "Choice 3",
    "Choice 4",
    "Exit",
};
static int n_choices = sizeof(choices)/sizeof(choices[0]);

static void print_menu(WINDOW *menu_win, int highlight, int color) {
    int x, y, i;

    x = 2;
    y = 4;
    wcolor_set(menu_win, color, NULL);
    //box(menu_win, 0, 0);
    for(i = 0; i < n_choices; ++i) {
        if (highlight == i) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        ++y;
    }
}

int main(void) {

    WINDOW* mainwin = initscr();
    if (mainwin == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    noecho();
    raw();
    keypad(mainwin, TRUE);

    initColors();

    int cols = 80;
    int rows = 25;
    getmaxyx(mainwin, rows, cols);
    int w_width = cols / 2;
    int w_height = rows / 2;;
    //int x = (cols - w_width)  / 2;
    //int y = (rows - w_height) / 2;

    mvprintw(1, 1, "max = %d,%d\n", cols, rows);

    WINDOW* child1 = create_window(mainwin, w_height, w_width, 3, 0, "Modules");
    WINDOW* child2 = create_window(mainwin, w_height, w_width, 3, w_width, "Declarations");
    refresh();

    WINDOW* current = child1;
    wcolor_set(current, 4, NULL);

    int highlight = 1;

    print_menu(current, highlight, 2);
    wrefresh(current);

    uint64_t duration = 0;

    int ch = 0;
    while ( (ch = getch()) != 'q' ) {
        switch (ch) {
        case 0x9:   // apparantly TAB key
            wcolor_set(current, 0, NULL);
            //wclrtoeol(current);   // does work, but also clears border piece
            mvwaddstr(current, 9, 10, "(passive)");
            //wdeleteln(current);   // removes line (window gets smaller)
            //werase(current);  // clears all content (including borders, etc)
            print_menu(current, highlight, 0);
            wrefresh(current);
            // switch window
            if (current == child1) current = child2;
            else current = child1;
            wcolor_set(current, 4, NULL);
            attron(A_BOLD);
            mvwaddstr(current, 9, 10, "Selected");
            attroff(A_BOLD);
            highlight = 0;
            //ncurses
            break;
        case KEY_UP:
           highlight--;
           if (highlight < 0) highlight = n_choices-1;
           break;
        case KEY_DOWN:
            highlight++;
            if (highlight == n_choices) highlight = 0;
           break;
        case 10:
            break;
        default:
            mvprintw(10, 2, "You pressed: 0x%x", ch);
            mvwprintw(current, 7, 2, "You pressed: 0x%0x", ch);
            break;
        }
        mvprintw(2, 1, "duration %lld usec", duration);
        mvprintw(8, 1, "Printed in Background (in main window)");
        mvprintw(8, 60, "Printed in Background (in main window)");
        print_menu(current, highlight, 2);
        uint64_t t1 = getCurrentTime();
        wrefresh(current);
        uint64_t t2 = getCurrentTime();
        duration = t2 - t1;
    }

    delwin(child1);
    delwin(child2);
    delwin(mainwin);
    endwin();
    refresh();
    return EXIT_SUCCESS;
}



