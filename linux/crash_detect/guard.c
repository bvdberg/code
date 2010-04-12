/*
    helper tool to make sure a program is running at all times.
    it starts the executable indicated by the first cmdline parameter
    and checks if it exited normally. If the program didn't, it will
    be restarted.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


struct processlist {
   pid_t pid;
   char executable[64];  //todo make dynamic
};

int main(int argc, char *argv[]) {

   struct processlist plist[10];
   pid_t child_pid;
   int status;
   int numprocs;
   int i;

   if(argc < 2) {
      printf("Usage: watcher <proc1> <proc2> .. <procn>\n");
      exit(1);
   }

   numprocs = argc - 1;
   //    strncpy(argv[0], "Mud Watchdog", 16);

   //phase 1 - start all processes (and put them in plist)
   for(i=1; i<argc; i++) {
      child_pid = fork();
      if(child_pid == 0) {  //child
         execvp(argv[i], NULL);
      } else {   //parent
         printf("Starting [%s] - pid %d\n", argv[i], child_pid);
         plist[i].pid = child_pid;
         strncpy(plist[i].executable, argv[i], 63);
      }
   }

   sleep(3);

   //phase 2 - wait until first one terminates
   while(numprocs > 0) {
      sleep(1);
      child_pid = wait(&status);

      if( WIFEXITED(status) == 0 ) { //child exits abnormally
         for(i=0; i<argc; i++) {
            if( plist[i].pid == child_pid ) {
               printf("Watcher: Process [%s] (pid %d) crashed!\n", plist[i].executable, plist[i].pid);
               plist[i].pid = 0;
               //numprocs--;
	       //restart after crash
               child_pid = fork();
               if(child_pid == 0) {  //child
                  execvp(argv[i], NULL);
               } else {   //parent
                  printf("Starting [%s] - pid %d\n", argv[i], child_pid);
                  plist[i].pid = child_pid;
                  strncpy(plist[i].executable, argv[i], 63);
               }
            }
         }
      } else {
         for(i=0; i<argc; i++) {
            if( plist[i].pid == child_pid ) {
               printf("Process [%s] (pid %d) stopped!\n", plist[i].executable, plist[i].pid);
               plist[i].pid = 0;
               numprocs--;
            }
         }

      }
   }

   return 1;
}


