   // Compile with 'gcc thisfile.c -lrt -Wall'
   #include <stdlib.h>
   #include <stdio.h>
   #include <sys/mman.h>	// Needed for mlockall()
   #include <unistd.h>		// needed for sysconf(int name);
   #include <malloc.h>
   #include <sys/time.h>	// needed for getrusage
   #include <sys/resource.h>	// needed for getrusage
   #include <pthread.h>
   #include <limits.h>
   
   #define PRE_ALLOCATION_SIZE (100*1024*1024) /* 100MB pagefault free buffer */
   #define MY_STACK_SIZE       (100*1024)      /* 100 kB is enough for now. */
   
   static void setprio(int prio, int sched)
   {
   	struct sched_param param;
   	// Set realtime priority for this thread
   	param.sched_priority = prio;
   	if (sched_setscheduler(0, sched, &param) < 0)
   		perror("sched_setscheduler");
   }
   
   void show_new_pagefault_count(const char* logtext, 
   			      const char* allowed_maj,
   			      const char* allowed_min)
   {
   	static int last_majflt = 0, last_minflt = 0;
   	struct rusage usage;
   
   	getrusage(RUSAGE_SELF, &usage);
   
   	printf("%-30.30s: Pagefaults, Major:%ld (Allowed %s), " \
   	       "Minor:%ld (Allowed %s)\n", logtext,
   	       usage.ru_majflt - last_majflt, allowed_maj,
   	       usage.ru_minflt - last_minflt, allowed_min);
   	
   	last_majflt = usage.ru_majflt; 
   	last_minflt = usage.ru_minflt;
   }
   
   static void prove_thread_stack_use_is_safe(int stacksize)
   {
   	volatile char buffer[stacksize];
   	int i;
   
   	/* Prove that this thread is behaving well */
   	for (i = 0; i < stacksize; i += sysconf(_SC_PAGESIZE)) {
   		/* Each write to this buffer shall NOT generate a 
   			pagefault. */
   		buffer[i] = i;
   	}
   
   	show_new_pagefault_count("Caused by using thread stack", "0", "0");
   }
   
   /*************************************************************/
   /* The thread to start */
   static void *my_rt_thread(void *args)
   {
   	struct timespec ts;
   	ts.tv_sec = 30;
   	ts.tv_nsec = 0;
   
   	setprio(sched_get_priority_max(SCHED_RR), SCHED_RR);
   
   	printf("I am an RT-thread with a stack that does not generate " \
   	       "page-faults during use, stacksize=%i\n", MY_STACK_SIZE);
   
   //<do your RT-thing here>
   
   	show_new_pagefault_count("Caused by creating thread", ">=0", ">=0");
   
   	prove_thread_stack_use_is_safe(MY_STACK_SIZE);
   
   	/* wait 30 seconds before thread terminates */
   	clock_nanosleep(CLOCK_REALTIME, 0, &ts, NULL);
   
   	return NULL;
   }
   
   /*************************************************************/
   
   static void error(int at)
   {
   	/* Just exit on error */
   	fprintf(stderr, "Some error occured at %d", at);
   	exit(1);
   }
   
   static void start_rt_thread(void)
   {
   	pthread_t thread;
   	pthread_attr_t attr;
   
   	/* init to default values */
   	if (pthread_attr_init(&attr))
   		error(1);
   	/* Set the requested stacksize for this thread */
   	if (pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN + MY_STACK_SIZE))
   		error(2);
   	/* And finally start the actual thread */
   	pthread_create(&thread, &attr, my_rt_thread, NULL);
   }
   
   static void configure_malloc_behavior(void)
   {
   	/* Now lock all current and future pages 
   	   from preventing of being paged */
   	if (mlockall(MCL_CURRENT | MCL_FUTURE))
   		perror("mlockall failed:");
   
   	/* Turn off malloc trimming.*/
   	mallopt(M_TRIM_THRESHOLD, -1);
   
   	/* Turn off mmap usage. */
   	mallopt(M_MMAP_MAX, 0);
   }
   
   static void reserve_process_memory(int size)
   {
   	int i;
   	char *buffer;
   
   	buffer = malloc(size);
   
   	/* Touch each page in this piece of memory to get it mapped into RAM */
   	for (i = 0; i < size; i += sysconf(_SC_PAGESIZE)) {
   		/* Each write to this buffer will generate a pagefault.
   		   Once the pagefault is handled a page will be locked in
   		   memory and never given back to the system. */
   		buffer[i] = 0;
   	}
   
   	/* buffer will now be released. As Glibc is configured such that it 
   	   never gives back memory to the kernel, the memory allocated above is
   	   locked for this process. All malloc() and new() calls come from
   	   the memory pool reserved and locked above. Issuing free() and
   	   delete() does NOT make this locking undone. So, with this locking
   	   mechanism we can build C++ applications that will never run into
   	   a major/minor pagefault, even with swapping enabled. */
   	free(buffer);
   }
   
   int main(int argc, char *argv[])
   {
   	show_new_pagefault_count("Initial count", ">=0", ">=0");
   
   	configure_malloc_behavior();
   
   	show_new_pagefault_count("mlockall() generated", ">=0", ">=0");
   
   	reserve_process_memory(PRE_ALLOCATION_SIZE);
   
   	show_new_pagefault_count("malloc() and touch generated", 
   				 ">=0", ">=0");
   
   	/* Now allocate the memory for the 2nd time and prove the number of
   	   pagefaults are zero */
   	reserve_process_memory(PRE_ALLOCATION_SIZE);
   	show_new_pagefault_count("2nd malloc() and use generated", 
   				 "0", "0");
   
   	printf("\n\nLook at the output of ps -leyf, and see that the " \
   	       "RSS is now about %d [MB]\n",
   	       PRE_ALLOCATION_SIZE / (1024 * 1024));
   
   	start_rt_thread();
   
   //<do your RT-thing>
   
   	printf("Press <ENTER> to exit\n");
   	getchar();
   
   	return 0;
   }
