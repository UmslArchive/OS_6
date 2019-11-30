//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #6
//Date:     11/30/19
//=========================================================

#include "interrupts.h"
#include "shared.h"

#define TICK_RATE 50            //ns
#define SPAWN_RATE 499999999    //ns

int main(int arg, char* argv[]) {

    //Counters and iterators
    int i, j, k;
    PCB* pcbIter;
    
    //Seed rand
    srand(time(NULL));

    //Register signal handlers
    ossInitSignalHandler();
    sigaction(SIGINT, &ossSigAction, 0);
    sigaction(SIGALRM, &ossSigAction, 0);
    sigaction(SIGTERM, &ossSigAction, 0);

    //Get semaphore pointer
    sem_t* shmSemPtr = 
        initShmSemaphore (
            SHM_KEY_SEM, 
            shmSemSize, 
            &shmSemID, 
            SHM_OSS_FLAGS
        );
    
    //Init semaphore
    if(sem_init(shmSemPtr, 1, 1) == -1) {
        perror("ERROR:sem_init failed");
        cleanupAll();
        exit(1);
    }

    //Get shared memory pointers
    Clock* shmClockPtr = 
        (Clock*)initSharedMemory (
            SHM_KEY_CLOCK, 
            shmClockSize, 
            &shmClockID, 
            SHM_OSS_FLAGS
        );

    PCB* shmPcbPtr = 
        (PCB*)initSharedMemory (
            SHM_KEY_PCB, 
            shmPcbSize, 
            &shmPcbID, 
            SHM_OSS_FLAGS
        );
    
    FrameTable* shmFrameTable = 
        (FrameTable*)initSharedMemory (
            ftKey,
            ftSize,
            &ftID,
            FT_OSS_FLAGS
        );

    //Init shared memory values
    initClock(shmClockPtr);
    ossInitPcbArray(shmPcbPtr);
    initFrameTable(shmFrameTable);

    //Message queue init
    int msgPid, msgReqType, msgReqAddr;
    char msgBuff[100];
    ossInitMessageQueue();

    //Generate first process random spawn time
    Clock spawnTime;
    spawnTime.nanoseconds = rand() % 499999999 + 1;
    spawnTime.seconds = 0;

    //Mark every second
    int prevSecond = shmClockPtr->seconds;

    //Child death stats
    long accessPerSecond, faultsPerAccess;
    Clock avgAccessSpeed;

    //-----

    while(!ossSignalReceivedFlag) {

        if(shmClockPtr->seconds > prevSecond) {
            fprintf(stderr, "\t\t\t\t");
            printClock(shmClockPtr);
            prevSecond = shmClockPtr->seconds;
            sleep(2);
        }

        //Process message
        msgPid = -1;
        msgReqType = -1;
        msgReqAddr = -1;
        ossReceiveMessage(&msgPid, &msgReqType, &msgReqAddr);

        if(msgPid != -1) {
            
            pcbIter = shmPcbPtr;
            pcbIter += getIndexOfPid(shmPcbPtr, msgPid);
            //printPcb(shmPcbPtr, getIndexOfPid(shmPcbPtr, msgPid));
            if(pcbIter->state == WAITING) {             
                if(msgReqType == READ) {
                    sprintf (
                        msgBuff, 
                        "%d APPROVED for READ at %d", 
                        msgPid, 
                        msgReqAddr
                    );
                }
                else {
                    sprintf (
                        msgBuff,
                        "%d APPROVED for WRITE at %d",
                        msgPid,
                        msgReqAddr
                    );
                }
                ossSendMessage(pcbIter->pid, msgBuff);
                pcbIter->state = READY;
            }
        }
        

        //Spawn process every 500ms
        if(checkIfPassedTime(shmClockPtr, &spawnTime)) {
            spawnProcess(shmPcbPtr);

            //Generate next spawn time
            setClock (
                &spawnTime,
                shmClockPtr->seconds,
                shmClockPtr->nanoseconds
            );
            advanceClock(&spawnTime, 0, rand() % SPAWN_RATE + 1);
        }

        //Advance the clock
        sem_wait(shmSemPtr);
            advanceClock(shmClockPtr, 0, TICK_RATE);
        sem_post(shmSemPtr);

        //Wait on dead child if there is one
        waitNoBlock(shmPcbPtr, &accessPerSecond, &faultsPerAccess, &avgAccessSpeed);
    }

    //-----

    //Signal too all child processes to finish up
    killChildren(shmPcbPtr);

    //Wait on remaining processes
    while(areActiveProcesses(shmPcbPtr) == 1)
        waitNoBlock(shmPcbPtr, &accessPerSecond, &faultsPerAccess, &avgAccessSpeed);

    //Cleanup shared memory and message queue
    cleanupAll();
}