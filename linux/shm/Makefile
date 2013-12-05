CC_ARGS=-Wall -g

all: shmserver shmclient

shmserver: shm_server.c
		gcc shm_server.c -o shmserver $(CC_ARGS)

shmclient: shm_client.c
		gcc shm_client.c -o shmclient $(CC_ARGS)

clean:
	rm -f shmserver shmclient
