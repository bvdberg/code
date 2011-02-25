#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>

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
	u_int64_t t1 = now();
    pid_t pid = fork();
	u_int64_t t2 = now();
    
    const char* name = (pid == 0) ? "child" : "parent";
    printf("[%d - %s] duration = %llu\n", (int) pid, name, t2 - t1);

	return 0;
}

