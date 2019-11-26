//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
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

#define MAX_CHILD_PROCESSES 18

typedef enum ps_state_enum {
    NULL_PS,
    READY
} State;

typedef struct pcb_struct {
    pid_t pid;
    State state;    
} PCB;

//Initialization
void ossInitPcbArray(PCB* pcbArray);
void initPcb(PCB* pcbIterator);

//oss process management functions:
int spawnProcess();
void waitNoBlock();
int areActiveProcesses();
void killChildren();

//Utility
void printPcb(PCB* pcbArray, PCB* pcbIterator);
void printPcbArray(PCB* pcbArray);
int getIndexOfPid(PCB* pcbArray, pid_t pid);

#endif