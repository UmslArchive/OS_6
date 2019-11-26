//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "interrupts.h"
#include "shared.h"

int main(int arg, char* argv[]) {
    int i, j, k;
    
    //Initializations:
    
    //Seed rand
    srand(time(NULL));

    //Register signal handlers
    ossInitSignalHandler();
    sigaction(SIGINT, &ossSigAction, 0);
    sigaction(SIGALRM, &ossSigAction, 0);
    sigaction(SIGTERM, &ossSigAction, 0);

    //Init Managers
    initOssProcessManager();

    //Init semaphore
    sem_t* shmSemPtr = initShmSemaphore(SHM_KEY_SEM, shmSemSize, &shmSemID, SHM_OSS_FLAGS);
    if(sem_init(shmSemPtr, 1, 1) == -1) {
        perror("ERROR:sem_init failed");
        cleanupAll();
        exit(1);
    }

    //Init shared memory pointers
    Clock* shmClockPtr = (Clock*)initSharedMemory(SHM_KEY_CLOCK, shmClockSize, &shmClockID, SHM_OSS_FLAGS);
    PCB* shmPcbPtr = (PCB*)initSharedMemory(SHM_KEY_PCB, shmPcbSize, &shmPcbID, SHM_OSS_FLAGS);

    //Init shared memory values
    initClock(shmClockPtr);
    ossInitPcbArray(shmPcbPtr);

    //Generate first random process spawn time
    Clock spawnTime;
    spawnTime.nanoseconds = rand() % 499999999 + 1;
    spawnTime.seconds = 0;
  
    while(1) {

        //Wait on dead child if there is one
        waitNoBlock();

        //Check if a signal was received
        if(ossSignalReceivedFlag == 1) {
            killChildren();
            break;
        }
    }

    //Wait on remaining processes
    while(areActiveProcesses() == 1)
        waitNoBlock();

    //Cleanup
    destroyProcessManager();
    cleanupAll();
}