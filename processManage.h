//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #6
//Date:     11/30/19
//=========================================================

#ifndef PROC_MAN_H
#define PROC_MAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#define MAX_CHILD_PROCESSES 18

typedef enum ps_state_enum {
    NULL_PS,
    INITIALIZING,
    READY,
    WAITING
} State;

typedef struct pcb_struct {
    pid_t pid;
    State state;    
} PCB;

//Initialization
void ossInitPcbArray(PCB* pcbArray);
void initPcb(PCB* pcbIterator);

//oss process management functions:
int spawnProcess(PCB* pcbArray);
void addToPcbArray(PCB* pcbArray, pid_t pid);
void waitNoBlock(PCB* pcbArray);
void removeFromPcbArray(PCB* pcbArray, pid_t pid);
int areActiveProcesses(PCB* pcbArray);
void killChildren(PCB* pcbArray);

//Utility
void printPcb(PCB* pcbArray, int position);
void printPcbArray(PCB* pcbArray);
int getIndexOfPid(PCB* pcbArray, pid_t pid);
int spawnDummyProcess(PCB* pcbArray, pid_t pid);

#endif