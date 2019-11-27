//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #6
//Date:     11/30/19
//=========================================================

#include "interrupts.h"
#include "shared.h"

int main(int arg, char* argv[]) {

    //Initializations:

    //Seed rand
    srand(time(NULL));

    //Register signal handler
    usrInitSignalHandler();
    sigaction(SIGTERM, &usrSigAction, 0);
    sigaction(SIGINT, &usrSigAction, 0);

    //Init shared memory pointers
    sem_t* shmSemPtr = initShmSemaphore(SHM_KEY_SEM, shmSemSize, &shmSemID, SHM_USR_FLAGS);
    Clock* shmClockPtr = (Clock*)initSharedMemory(SHM_KEY_CLOCK, shmClockSize, &shmClockID, SHM_USR_FLAGS);
    PCB* shmPcbPtr = (PCB*)initSharedMemory(SHM_KEY_PCB, shmPcbSize, &shmPcbID, SHM_USR_FLAGS);

    //Init message queue
    usrInitMessageQueue();

    //Clock which dictates request times
    Clock reqTime;
    reqTime.nanoseconds = 0;
    reqTime.seconds = 0;

    //Create message string
    char msgBuff[100];
    sprintf(msgBuff, "%d hello msg", getpid());

    while(1) {
        //Spawn process every 500ms
        if(checkIfPassedTime(shmClockPtr, &reqTime) == 1) {

            printClock(shmClockPtr);
            sendMessage(rand() % 10 + 1, msgBuff);
            //printPcbArray(shmPcbPtr);

            fprintf(stderr, "\n");

            //spawned1 = 1;

            //Generate next spawn time
            setClock(&reqTime,
                shmClockPtr->seconds,
                shmClockPtr->nanoseconds);
            
            advanceClock(&reqTime, 0, rand() % 499999999 + 1);
        }
        
        //Check if a signal was received
        if(usrSignalReceivedFlag == 1)
            break;
    }

    detachAll();
    //fprintf(stderr, "DEATH %d\n", getpid());
    return 50;
}