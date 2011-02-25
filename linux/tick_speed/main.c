#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define u_int64_t unsigned long long

static u_int64_t now()
{
	struct timeval val;
	int err = gettimeofday(&val, NULL);
	if (err) return 0;
	u_int64_t result = val.tv_sec * 1000000;
	result += val.tv_usec;
	return result;
}


int main(int argc, char* argv[])
{
	int delay = 1;
	if (argc == 2) delay = atoi(argv[1]);
	printf("using delay %d usec\n", delay);

	u_int64_t t1 = now(); 		
	u_int64_t t2 = now(); 		
	printf("duration = %llu\n", t2-t1);

	u_int64_t t3 = now(); 		
	usleep(delay);
	u_int64_t t4 = now(); 		
	printf("duration = %llu\n", t4-t3);
	
	return 0;
}

