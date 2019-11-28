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

void ossSendMessage(long pid, const char* text) {
    struct Msg newMsg;
    int len = strlen(text) + 1;
    newMsg.msgType = pid;
    memcpy((char*)newMsg.msgText, (char*)text, len);
    msgsnd(msgID, &newMsg, len, 0);
}

void ossReceiveMessage() {
    struct Msg rcvMsg;
    int success = msgrcv(msgID, &rcvMsg, sizeof(rcvMsg.msgText), 100000, IPC_NOWAIT);
    if(success != -1) {
        fprintf(stderr, "OSS received msg: %s type(%ld)\n", rcvMsg.msgText, rcvMsg.msgType);
    }    
}

void usrSendMessage(const char* text) {
    struct Msg newMsg;
    newMsg.msgType = 100000;
    int len = strlen(text) + 1;
    memcpy((char*)newMsg.msgText, (char*)text, len);
    msgsnd(msgID, &newMsg, len, 0);
}

void usrReceiveMessage(long pid) {
    struct Msg rcvMsg;
    int success = msgrcv(msgID, &rcvMsg, sizeof(rcvMsg.msgText), pid, IPC_NOWAIT);
    if(success != -1) {
        fprintf(stderr, "USR %ld received msg: %s type(%ld)\n", pid, rcvMsg.msgText, rcvMsg.msgType);
    }
}