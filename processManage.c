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

static void addToActiveProcesses() {
    
}

static void removeFromActiveProccesses(pid_t process) {

}

int areActiveProcesses() {
    return 0;
}

static int activeProcessArrayFull(){

}

void printActiveProcessArray() {

}

//Initialization/deallocation:

int initOssProcessManager() {
    
}

int destroyProcessManager() {

}


//oss functions:

int spawnProcess(int* newestChildPid) {

}

int spawnDummyProcess() {

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

//Utility
int getIndexOfPid(pid_t searchPid) {
}

pid_t getPidOfIndex(int index) {
}


int getNumActivePs() {
    return (int)numActivePs;
}