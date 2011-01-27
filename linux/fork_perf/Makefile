all: fork_perf thread_perf
fork_perf: main.c
	gcc main.c -o fork_perf -Wall -DUSE_FORK

thread_perf: main.c
	gcc main.c -o thread_perf -Wall -DUSE_PTHREAD -lpthread
