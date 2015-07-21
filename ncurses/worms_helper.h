/*

  HELPER.H
  ========
  (c) Copyright Paul Griffiths 2000
  Email: mail@paulgriffiths.net

  Interface to helper functions.

*/


#ifndef PG_HELPER_H
#define PG_HELPER_H


/*  Macros  */

#define USER_QUIT    1
#define HITSELF      2
#define HITWALL      3


/*  Function prototypes  */

void Error_Quit(char * msg);
void Quit(int reason);
void GetTermSize(int * rows, int * cols);
void handler(int signum);
void ChangeDir(int d);


#endif  /*  PG_HELPER_H  */
