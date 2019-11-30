//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #6
//Date:     11/30/19
//=========================================================

#include "interrupts.h"
#include "shared.h"

int main(int arg, char* argv[]) {

    //Seed rand
    char* seedOffsetStr = argv[1];
    int seedOffset = atoi(seedOffsetStr);

    srand(time(NULL) + seedOffset);

    //Register signal handler
    usrInitSignalHandler();
    sigaction(SIGTERM, &usrSigAction, 0);
    sigaction(SIGINT, &usrSigAction, 0);

    //Get shared memory pointers
    sem_t* shmSemPtr = 
        initShmSemaphore (
            SHM_KEY_SEM,
            shmSemSize,
            &shmSemID,
            SHM_USR_FLAGS
        );

    Clock* shmClockPtr = 
        (Clock*)initSharedMemory (
            SHM_KEY_CLOCK,
            shmClockSize,
            &shmClockID,
            SHM_USR_FLAGS
        );

    PCB* shmPcbPtr = 
        (PCB*)initSharedMemory (
            SHM_KEY_PCB,
            shmPcbSize,
            &shmPcbID,
            SHM_USR_FLAGS
        );

    //Get PCB index and iterator
    int pcbIndex = getIndexOfPid(shmPcbPtr, getpid());
    PCB* pcbIterator = shmPcbPtr + pcbIndex;

    //Connect message queue
    usrInitMessageQueue();

    //Clock which dictates request times
    Clock reqTime;
    reqTime.nanoseconds = 0;
    reqTime.seconds = 0;

    //Message buffer
    char msgBuff[100];
    int readOrWrite = 0;

    //Set state to ready
    pcbIterator->state = READY;

    //-----
    int referenceCount = 0;

    while(!usrSignalReceivedFlag) {

        usrReceiveMessage((long)getpid());

        //Send request if it is time
        if(checkIfPassedTime(shmClockPtr, &reqTime) == 1) {
            readOrWrite = rand() % 2;
            if(readOrWrite == 0) {
                sprintf(msgBuff, "%d,READ,%d", getpid(), rand() % 100);
            }
            else {
                sprintf(msgBuff, "%d,WRITE,%d",  getpid(), rand() % 100);
            }
            
            usrSendMessage(msgBuff);
            pcbIterator->state = WAITING;
            while(pcbIterator->state == WAITING && !usrSignalReceivedFlag);
            referenceCount++;

            fprintf(stderr, "%d has made %d refs\n", getpid(), referenceCount);

            //Generate next request time
            setClock (
                &reqTime,
                shmClockPtr->seconds,
                shmClockPtr->nanoseconds
            );
            advanceClock(&reqTime, 1, rand() % 50000);
        }
    }

    //-----

    detachAll();

    return 0;
}