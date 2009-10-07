//client that sends messages on a queue

#include "queue.h"

int main(int argc, char *argv[]) {

    if(argc != 2) {
	printf("usage: client <id>\n");
	exit(1);
    }
    int id = atoi(argv[1]);
    if(id == KERNEL_ID) {
	printf("ID taken by kernel, choose another one!\n");
	exit(1);
    }
    
    Queue *queue = new Queue();
    int key = ftok("/tmp/key", 'a');
    int result = queue->open(key,false);
    if(result == -1) {
	printf("Client: error opening queue\n");
	exit(1);
    }
    
    struct msg_struct msg_in, msg_out;
   
    int count = 0;
    while (count<10) {
	//send message to server
	msg_out.mtype = KERNEL_ID;
	msg_out.srcID = id;
	msg_out.value = count;
	result = queue->send(&msg_out);
	if(result == -1) {
	    printf("Client: error sending msg\n");
	    exit(1);
	}
	printf("Client: SEND mtype=%d value = %d\n", msg_out.mtype, msg_out.value);

	//wait for answers
	result = queue->recv(id, &msg_in, true);
        if(result == -1) {
	    printf("Client: lost connection to queue\n");
	    exit(1);
	}
	printf("Client: RECV, value = %d\n", msg_in.value);

	count++;
	sleep(1);
    }
    //queue->close();

    return 1;
}

