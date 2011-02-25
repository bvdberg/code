#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> // Needed for mlockall()
#include <unistd.h> // needed for sysconf(int name);
#include <malloc.h>
#include <sys/time.h> // needed for getrusage
#include <sys/resource.h> // needed for getrusage


#define SOMESIZE (100*1024) // 100kB


int main(int argc, char* argv[])
{
   // Now lock all current and future pages from preventing of being paged
   if (mlockall(MCL_CURRENT | MCL_FUTURE ))
   {
       perror("mlockall failed:");
       exit(1);
   }
   
   int page_size = sysconf(_SC_PAGESIZE);
   // Allocate some memory
   char* buffer = malloc(SOMESIZE);
   
   // Touch each page in this piece of memory to get it mapped into RAM
   int i;
   for (i=0; i < SOMESIZE; i+=page_size)
   {
       // Each write to this buffer will generate a pagefault.
       // Once the pagefault is handled a page will be locked in memory and never
       // given back to the system.
       buffer[i] = 0;
       // print the number of major and minor pagefaults this application has triggered
       struct rusage usage;
       getrusage(RUSAGE_SELF, &usage);
       (int)
       printf("Major-pagefaults:%d, Minor Pagefaults:%d\n", (int)usage.ru_majflt, (int)usage.ru_minflt);
   }
   // buffer is never released, or swapped, so using it from now will never lead to any pagefault
   

   //<do your RT-thing>
   

   return 0;
}
