CC_ARGS=-Wall -g 

all: shmserver shmclient

shmserver: shm_server.c 
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-gcc shm_server.c -o shmserver $(CC_ARGS) 

shmclient: shm_client.c
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-gcc shm_client.c -o shmclient $(CC_ARGS) 

test: shmserver shmclient
		cp shmserver /home/basberg/shmserver
		cp shmclient /home/basberg/shmclient

clean:
	rm -f shmserver shmclient
