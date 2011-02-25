#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>

#define u_int64_t unsigned long long

static u_int64_t now()
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	u_int64_t result = now.tv_sec;
	result *= 1000000;
	result += (now.tv_nsec / 1000);
	return result;
}


class Element {
public:
	Element(int id_) : id(id_) {}
	int getID() const { return id; }
private:
	int id;
	int buffer[1000];
};


class Database {
public:
	Database() : count(0), timestamp(now()) {}
	~Database() {}
	void add(Element* elem) {
		count++;
		u_int64_t t1 = now();
		u_int64_t diff = t1 - timestamp;
		//printf("diff = %llu\n", diff);
		//printf("  ts = %llu\n", t1);
		if (diff > 500000) {
			int size = count * sizeof(Element) / 1024;
			printf("DB: added elem %d  (%d elements, %d kB\n", elem->getID(), count, size);
			timestamp = t1;
		}
		// leak
	}
private:
	int count;
	u_int64_t timestamp;
};


void doIteration(Database& db, int i)
{
	db.add(new Element(i));	
}


void run(int times, int delay)
{
	Database db;
	int total = 0;
	int i;
	for (i=0; i<times; i++) {
		total += sizeof(Element);
		doIteration(db, i);
		usleep(delay * 1000);
	}
}


int main(int argc, char* argv[])
{
	if (argc != 3) {

		printf("Usage: memtest [times] [delay in ms]\n");
		return -1;
	}
	
	pid_t pid = getpid();
	printf("PID = %d\n", (int)pid);
	printf("sizeof(Element) = %d\n", sizeof(Element));

	int times = atoi(argv[1]);
	int delay = atoi(argv[2]);
	run(times, delay);

	return 0;
}

