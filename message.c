//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #6
//Date:     11/30/19
//=========================================================

#include "message.h"

const key_t msgKey = 0x11111111;
int msgID = 0;
const size_t msgSize = sizeof(struct Msg) - sizeof(long);
const int MSG_OSS_FLAGS = 0666 | IPC_CREAT;
const int MSG_USR_FLAGS = 0666;

void ossInitMessageQueue() {
    msgID = msgget(msgKey, MSG_OSS_FLAGS);
    if(msgID == -1) {
        perror("ERROR: msgget");
    }
}

void usrInitMessageQueue() {
    msgID = msgget(msgKey, MSG_USR_FLAGS);
    if(msgID == -1) {
        perror("ERROR: msgget");
    }
}

void destroyMessageQueue() {
    msgctl(msgID, IPC_RMID, NULL);
}

void sendMessage(long type, const char* text) {
    struct Msg newMsg;

    newMsg.msgType = type;

    int len = strlen(text) + 1;
    fprintf(stderr, "len = %d\n", len);

    memcpy((char*)newMsg.msgText, (char*)text, len);

    int i;
    for(i = 0; i < len; ++i) {
        fprintf(stderr, "%c", newMsg.msgText[i]);
    }
}