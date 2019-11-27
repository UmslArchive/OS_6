//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #6
//Date:     11/30/19
//=========================================================

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define _XOPEN_SOURCE //Needed for sigaction struct

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

extern int ossSignalReceivedFlag;
extern int usrSignalReceivedFlag;
extern int ossPauseFlag;

extern struct sigaction ossSigAction;
extern struct sigaction usrSigAction;

void ossInitSignalHandler();
void usrInitSignalHandler();

void ossSignalHandler(int signal);
void usrSignalHandler(int signal);

#endif