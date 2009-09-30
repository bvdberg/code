#ifndef MUD_QUEUE_H
#define MUD_QUEUE_H

#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "types.h"

using namespace std;

class Queue {
    int qid;
    
   public:
      Queue() { }
      int open(const key_t key, const bool nieuw);
      int close();
      int send(struct msg_struct *msg);
      int recv(const long type, struct msg_struct *msg, const bool block);
      int length() const;
};

#endif

