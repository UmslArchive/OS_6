//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #6
//Date:     11/30/19
//=========================================================

#include "interrupts.h"
#include "shared.h"

int main(int arg, char* argv[]) {

    //Seed rand
    srand(time(NULL));

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

    //Create message string
    char msgBuff[100];
    sprintf(msgBuff, "\"usr %d says hello\"", getpid());

    //-----

    while(!usrSignalReceivedFlag) {

        usrReceiveMessage((long)getpid());

        //Send request if it is time
        if(checkIfPassedTime(shmClockPtr, &reqTime) == 1) {
            
            usrSendMessage(msgBuff);

            //Generate next request time
            setClock (
                &reqTime,
                shmClockPtr->seconds,
                shmClockPtr->nanoseconds
            );
            advanceClock(&reqTime, 0, rand() % 499999999 + 1);
        }
    }

    //-----

    detachAll();

    return 50;
}