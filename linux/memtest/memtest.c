#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("Usage: memtest [times] [amount in kB]\n");
		return -1;
	}
	int times = atoi(argv[1]);
	int amount = atoi(argv[2]);
	int total = 0;
	int i;
	for (i=0; i<times; i++) {
		total += amount;
		void* buffer = calloc(amount, 1024);
		if (buffer == NULL) {
			printf("alloc failed\n");
			break;
		}
        memset(buffer, 0, amount*1024);
		printf("allocated %d kB, total %8d kB\n", amount, total);
	}
    sleep (20);

	return 0;
}

