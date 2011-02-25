#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h> // Needed for mlockall()
#include <unistd.h> // needed for sysconf(int name);
#include <malloc.h>
#include <sys/time.h> // needed for getrusage
#include <sys/resource.h> // needed for getrusage


#define SOMESIZE (100*1024*1024) // 100MB


int main(int argc, char* argv[])
{
   // Now lock all current and future pages from preventing of being paged
   if (mlockall(MCL_CURRENT | MCL_FUTURE ))
   {
       perror("mlockall failed:");
   }
   
   // Turn off malloc trimming.
   mallopt (M_TRIM_THRESHOLD, -1);

   // Turn off mmap usage.
   mallopt (M_MMAP_MAX, 0);

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
   free(buffer);
   // buffer is now released. As glibc is configured such that it never gives back memory to
   // the kernel, the memory allocated above is locked for this process. All malloc() and new()
   // calls come from the memory pool reserved and locked above. Issuing free() and delete()
   // does NOT make this locking undone. So, with this locking mechanism we can build C++ applications
   // that will never run into a major/minor pagefault, even with swapping enabled.
   

   //<do your RT-thing>
   

   return 0;
}
