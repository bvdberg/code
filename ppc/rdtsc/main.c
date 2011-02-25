#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

#include "rdtsc.h"

// cat/proc/cpuinfo shows timebase of 66660000
const unsigned long long TIMEBASE_SEC = 66660000;
const unsigned long long TIMEBASE_MS = 66660;

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



static unsigned long long cpuOffsetMs(unsigned long long base)
{
	unsigned long long offset = rdtsc();
	unsigned long long tt = offset - base;
	return tt / TIMEBASE_MS;
}


int main(int argc, char* argv[])
{
/*
	printf("timebase = %llu\n", TIMEBASE);
	unsigned long long a, b;

	a = rdtsc();
	b = rdtsc();
	printf("%llu\n", b - a);

	unsigned long long t1 = rdtsc();
	usleep(100000);
	unsigned long long t2 = rdtsc();
	printf("100 ms: %llu\n", t2 - t1);

	unsigned long long t3 = rdtsc();
	sleep(1);
	unsigned long long t4 = rdtsc();
	printf("1 sec: %llu\n", t4 - t3);

	printf("rdtsc() = %llu\n", t4);
*/

	if (argc != 2) {
		printf("Usage: %s [delay in ms]\n", argv[0]);
		return -1;
	}
	int delay = atoi(argv[1]);

	u_int64_t timestamp = now();
	unsigned long long base = rdtsc();
	u_int64_t start = timestamp;
	while (1) {
		u_int64_t t1 = now();
		if (t1 >= timestamp) {
			u_int64_t diff = t1 - timestamp;
			printf("diff = %llu\n", diff);
		} else {
			u_int64_t diff = timestamp - t1;
			printf("TIME WENT BACK! %llu\n", diff); 
		}
		u_int64_t dur_ts = t1 - start;
		unsigned long long dur_tsc = cpuOffsetMs(base);
		printf("dur dt = %llu  tsc = %llu\n", dur_ts / 1000, dur_tsc);
		//printf("  ts = %llu\n", t1);
		timestamp = t1;
		usleep(delay * 1000);
	}

	return 0;
}

