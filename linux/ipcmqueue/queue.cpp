/*
    Queue: queue is the low-lvl mechanism for using IPC-message queues.
    It offers send and (blocking/non-blocking) receive functions.
 
    Copyright (C) 2002 B. van den Berg
 
    Version: alpha 0.1
*/

#include "queue.h"

int Queue::open(const key_t key, const bool nieuw) {
   if(nieuw) {
      msgctl(msgget(key, 0660), IPC_RMID, 0); //close old queue
      qid = msgget (key, IPC_CREAT|IPC_EXCL|0660); //open new one
   } else {
      qid = msgget (key, 0660);
   }
   return qid;
}

int Queue::close() {
   return msgctl(qid, IPC_RMID, 0);
}

int Queue::send(struct msg_struct *msg) {
   int result, length;
   //TODO only calculate once
   length = sizeof(msg_struct)-sizeof(long);
   result = msgsnd(qid, msg, length, 0);

   return result;
}

int Queue::recv(long type, struct msg_struct *msg, const bool block) {
   int result, length;
   //TODO only calculate once
   length = sizeof(msg_struct)-sizeof(int);
   
   if(block)
      result = msgrcv(qid, msg, length, type, 0);
   else
      result = msgrcv(qid, msg, length, type, IPC_NOWAIT);
   return result;
}

/* returns the number of messages in the queue. NOTE: doesn't work?  */
int Queue::length() const {
   struct msqid_ds info;
   msgctl(qid, IPC_STAT, &info);

   return info.msg_qnum;
}

