#ifndef MSG_TYPES_H
#define MSG_TYPES_H

#define KERNEL_ID 1

struct msg_struct {
   int mtype;  // who gets the message
   int srcID;
   int  value;
};

#endif

