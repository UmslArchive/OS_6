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

    //Stats and death
    int referenceCount = 0;
    int pageFaults = 0;
    Clock totalAccessTime;
    initClock(&totalAccessTime);
    int dieCheck = 1000 + (rand() % 200) - 100;
    int die;

    while(!usrSignalReceivedFlag) {

        usrReceiveMessage((long)getpid());

        //Send request if it is time
        if(checkIfPassedTime(shmClockPtr, &reqTime) == 1) {

            //Death check
            if(referenceCount % dieCheck == 0 && referenceCount > 0) {
                die = rand() % 15;
                fprintf(stderr, "die %d\n", die);
                if(die == 0)
                    break;
            }

            readOrWrite = rand() % 2;
            if(readOrWrite == 0) {
                sprintf(msgBuff, "%d,READ,%d", getpid(), rand() % MAX_PROCESS_SIZE);
            }
            else {
                sprintf(msgBuff, "%d,WRITE,%d",  getpid(), rand() % MAX_PROCESS_SIZE);
            }
            
            pcbIterator->state = WAITING;
            usrSendMessage(msgBuff);
            while(pcbIterator->state == WAITING && !usrSignalReceivedFlag);
            referenceCount++;

            /* if(!usrSignalReceivedFlag)
                fprintf(stderr, "%d has made %d refs\n", getpid(), referenceCount); */

            //Generate next request time
            setClock (
                &reqTime,
                shmClockPtr->seconds,
                shmClockPtr->nanoseconds
            );
            advanceClock(&reqTime, 0, rand() % 500000);
        }
    }

    //-----

    sendDeathMessage("1,2,3,4");

    detachAll();

    return 0;
}