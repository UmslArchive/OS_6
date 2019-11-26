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

//Initialization/deallocation
int initOssProcessManager();
int destroyProcessManager();

//oss process management functions:
int spawnProcess();
void waitNoBlock();
int areActiveProcesses();
void killChildren();

//Utility
void printActiveProcessArray();
int getIndexOfPid(pid_t searchPid);
pid_t getPidOfIndex(int index);
int spawnDummyProcess();

#endif