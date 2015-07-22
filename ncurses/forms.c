#include <form.h>

#include <ctype.h>

static FIELD* field[3];
static FORM* my_form;

static const char* options[] = {
    "ananas",
    "apples",
    "bananas",
    "cherry",
    "coconut",
    "lemon",
    "melon",
    "orange",
    "peach",
    "pear",
};
static const unsigned num_options = sizeof(options) / sizeof(options[0]);

static void printBuffer(int y, int x) {
    //form_driver(my_form, REQ_NEXT_FIELD);
    //form_driver(my_form, REQ_PREV_FIELD);
    form_driver(my_form, REQ_END_LINE);
    attron(COLOR_PAIR(1));
    mvprintw(10, 10, "Left [%s]   Right [%s]  ", field_buffer(field[0], 0), field_buffer(field[1], 0));
    attroff(COLOR_PAIR(1));
    form_driver(my_form, REQ_END_LINE);
}

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1,  COLOR_GREEN, COLOR_BLACK);

    field[0] = new_field(1, 10, 4, 18, 0, 0);
    field[1] = new_field(1, 10, 6, 18, 0, 0);
    field[2] = NULL;

    /* Set field options */
    set_field_back(field[0], A_UNDERLINE);  /* Print a line for the option  */
    field_opts_off(field[0], O_AUTOSKIP);   /* Don't go to next field when this Field is filled up */
    set_field_back(field[1], A_UNDERLINE);
    field_opts_off(field[1], O_AUTOSKIP);
    /* Create the form and post it */
    my_form = new_form(field);
    post_form(my_form);
    refresh();

    mvprintw(4, 10, "Value 1:");
    mvprintw(6, 10, "Value 2:");

    printBuffer(8, 10);

    for (unsigned i=0; i<num_options; ++i) {
        mvprintw(12+i, 10, "%s", options[i]);
    }
    form_driver(my_form, REQ_FIRST_FIELD);
    refresh();

    int ch;
    while((ch = getch()) != KEY_F(1))
    {   switch(ch)
        {   case KEY_DOWN:
                form_driver(my_form, REQ_NEXT_FIELD);
                form_driver(my_form, REQ_END_LINE);
                break;
            case KEY_UP:
                form_driver(my_form, REQ_PREV_FIELD);
                form_driver(my_form, REQ_END_LINE);
                break;
            case 0x7F:  // backspace
                form_driver(my_form, REQ_PREV_CHAR);
                form_driver(my_form, REQ_DEL_CHAR);
                break;
            default:
                mvprintw(8, 10, "You typed 0x%x", ch);
                // If this is a normal character, it gets Printed
                form_driver(my_form, ch);
                printBuffer(10, 10);
                refresh();
                break;
        }
    }

    /* Un post form and free the memory */
    unpost_form(my_form);
    free_form(my_form);
    free_field(field[0]);
    free_field(field[1]);

    endwin();
    return 0;
}

