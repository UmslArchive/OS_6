//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "processManage.h"

static pid_t pid = 0;
static int exitStatus = 0;
static int numActivePs = 0;

//Static functions:

static PCB* scanForEmptyPcbSlot(PCB* pcbArray) {
    if(pcbArray == NULL) {
        fprintf(stderr, "ERROR: Couldn't scan for empty--nullptr\n");
        return NULL;
    }

    PCB* iterator = pcbArray;
    int i;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        if(iterator->state == NULL_PS){
            return iterator;
        }
        iterator++;
    }

    return NULL;
}

int areActiveProcesses() {
    return 0;
}

static void addProcessToPcbArray(PCB* pcbArray, pid_t pid) {

}

static void removeFromActiveProccesses(pid_t process) {
    //Scan PCB array for pid
}


//Initialization/deallocation:
void initPcb(PCB* pcbIterator) {
    pcbIterator->pid = 0;
    pcbIterator->state = NULL_PS;
}

void ossInitPcbArray(PCB* pcbArray) {
    if(pcbArray == NULL) {
        fprintf(stderr, "ERROR: PCB Array init--nullptr\n");
        return;
    }

    int i;
    PCB* iterator = pcbArray;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        initPcb(iterator);
        iterator++;
    }
}

//oss functions:

int spawnProcess(int* newestChildPid) {

}

void waitNoBlock() {
    while((pid = waitpid(-1, &exitStatus, WNOHANG))) {
        if((pid == -1) && (errno != EINTR))
            break;
        else {
            //fprintf(stderr, "PID %d exited w/ status %d\n", pid, WEXITSTATUS(exitStatus));
            removeFromActiveProccesses(pid);
            --numActivePs;
            //printActiveProcessArray();
        }
    }
}

void killChildren() {
}

//Utility:
void printPcb(PCB* pcbArray, int position) {
    if(pcbArray == NULL) {
        fprintf(stderr, "ERROR: Couldn't print PCB--nullptr\n");
        return;
    }

    PCB*  iterator = (pcbArray + position);

    fprintf(stderr, "PCB#%.2d: pid(%.5d), state(%d)\n",
        position,
        (int)iterator->pid,
        (int)iterator->state
    );
}

void printPcbArray(PCB* pcbArray) {
    if(pcbArray == NULL) {
        fprintf(stderr, "ERROR: Couldn't print PCB array--nullptr\n");
        return;
    }

    PCB* iterator = pcbArray;
    int i;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        fprintf(stderr, "PCB#%.2d: pid(%.5d), state(%d)\n", 
            i, 
            (int)iterator->pid,
            (int)iterator->state
        );
        iterator++;
    }

}

int getIndexOfPid(PCB* pcbArray, pid_t pid) {
    if(pcbArray == NULL) {
        fprintf(stderr, "ERROR: Couldn't fetch index of pid--nullptr\n");
        return -1;
    }

    if((int)pid <= 0) {
        fprintf(stderr, "ERROR: Couldn't fetch index of pid--invalid pid\n");
        return -1;
    }

    PCB* iterator = pcbArray;
    int i;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        if(iterator->pid == pid) {
            return i;
        }
        iterator++;
    }

    return -1;
}

int spawnDummyProcess(PCB* pcbArray) {
    if(pcbArray == NULL) {
        fprintf(stderr, "ERROR: Couldn't spawn dummy--nullptr\n");
        return -1;
    }

    PCB* iterator = scanForEmptyPcbSlot(pcbArray);
    if(iterator == NULL) {
        fprintf(stderr, "ERROR: Couldn't spawn dummy--No Empty\n");
        return -1;
    }

    iterator->state = READY;
    iterator->pid = rand() % 99999;
}