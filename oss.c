//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #6
//Date:     11/30/19
//=========================================================

#include "interrupts.h"
#include "shared.h"

#define TICK_RATE 1000

int main(int arg, char* argv[]) {

    //Counters and iterators
    int i, j, k;
    PCB* pcbIter;
    
    //Initializations:
    
    //Seed rand
    srand(time(NULL));

    //Register signal handlers
    ossInitSignalHandler();
    sigaction(SIGINT, &ossSigAction, 0);
    sigaction(SIGALRM, &ossSigAction, 0);
    sigaction(SIGTERM, &ossSigAction, 0);

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

    //Message Queue init
    ossInitMessageQueue();

    //Generate first random process spawn time
    Clock spawnTime;
    spawnTime.nanoseconds = rand() % 499999999 + 1;
    spawnTime.seconds = 0;

    while(1) {

        ossReceiveMessage();

        //Spawn process every 500ms
        if(checkIfPassedTime(shmClockPtr, &spawnTime)) {
            spawnProcess(shmPcbPtr);

            pcbIter = shmPcbPtr;
            for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
                if(pcbIter->state != NULL_PS) {
                    ossSendMessage(pcbIter->pid, "\"oss says hello\"");
                    pcbIter++;
                }
            }

            //Generate next spawn time
            setClock (
                &spawnTime,
                shmClockPtr->seconds,
                shmClockPtr->nanoseconds
            );
            advanceClock(&spawnTime, 0, rand() % 499999999 + 1);
        }

        //Advance the clock
        sem_wait(shmSemPtr);
            advanceClock(shmClockPtr, 0, TICK_RATE);
        sem_post(shmSemPtr);

        //Wait on dead child if there is one
        waitNoBlock(shmPcbPtr);

        //Check if a signal was received
        if(ossSignalReceivedFlag == 1) {
            killChildren(shmPcbPtr);
            break;
        }
    }

    //Wait on remaining processes
    while(areActiveProcesses(shmPcbPtr) == 1)
        waitNoBlock(shmPcbPtr);

    //printPcbArray(shmPcbPtr);

    //Cleanup
    cleanupAll();
    destroyMessageQueue();
}