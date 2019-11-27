//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #6
//Date:     11/30/19
//=========================================================

#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MSG_LEN 100

struct Msg {
    long msgType;
    char msgText[MAX_MSG_LEN];
};

extern const key_t msgKey;
extern int msgID;
extern const size_t msgSize;
extern const int MSG_OSS_FLAGS;
extern const int MSG_USR_FLAGS;

void ossInitMessageQueue();
void usrInitMessageQueue();
void destroyMessageQueue();
void sendMessage(long type, const char* text);
void receiveMessage();
#endif