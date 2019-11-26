//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
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

    int count = 0;
    while(1) {
        
        //Check if a signal was received
        if(usrSignalReceivedFlag == 1)
            break;

        count++;
    }

    detachAll();
    //fprintf(stderr, "DEATH %d\n", getpid());
    return 50;
}