//Server that listens on a queue

#include "queue.h"

int main(int argc, char *argv[]) {

    int id = KERNEL_ID;

    Queue *queue = new Queue();
    int key = ftok("/tmp/key", 'a');

    int result = queue->open(key,true);
    if(result == -1) {
	printf("Server: error opening queue\n");
	exit(1);
    }

    struct msg_struct msg_in, msg_out;
    int count = 1;
    while (count<=100) {
	//wait for messages
	result = queue->recv(id, &msg_in, true);
	if(result == -1) {
	    printf("Server: lost connection to queue\n");
	    exit(1);
	}
	printf("Server: (%3d) RECV srcID=%d value = %d\n", count, msg_in.srcID, msg_in.value);

	//send answer to client
	msg_out.mtype = msg_in.srcID;
	msg_out.srcID = id;
	msg_out.value = msg_in.value*10;
	result = queue->send(&msg_out);
	if(result == -1) {
	    printf("Server: error sending msg\n");
	    exit(1);
	}
	printf("Server: (%3d) SEND mtype=%d, src=%d, value=%d\n", count, msg_out.mtype, id, msg_out.value);
	count++;
    }
    
    queue->close();
    return 1;
}

