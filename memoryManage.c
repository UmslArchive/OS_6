//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #6
//Date:     11/30/19
//=========================================================

#include "memoryManage.h"

const key_t ftKey = 0x43214321;
int ftID = 0;
const size_t ftSize = sizeof(FrameTable);
const int FT_OSS_FLAGS = 0666 | IPC_CREAT;;
const int FT_USR_FLAGS = 0666;

//-----

void initPageTable(PageTable* pageTable) {
    int i;
    for(i = 0; i < PT_SIZE; ++i) {
        pageTable->table[i] = -1;
    }
}

void initFrameTable(FrameTable* frameTable) {
    int i;
    for(i = 0; i < FT_SIZE; ++i) {
        frameTable->table[i].dirty = 0;
        frameTable->table[i].page = -1;
        frameTable->table[i].pid = -1;
        frameTable->table[i].ref = 0;
        initClock(&frameTable->table[i].timestamp);
    }
}

void destroyPageTable() {
}

void destroyFrameTable() {
    if(ftID > 0)
        shmctl(ftID, IPC_RMID, NULL);
}

void pageFault(FrameTable* frameTable) {

}

void addPageToFrameTable(FrameTable* frameTable, long page, int pid, Clock timestamp, long ref) {

}

void makeDirty(FrameTable* frameTable, long page, int pid) {

}

void removePageFromFrameTable(FrameTable* frameTable, long page, int pid) {

}

int getIndexOfPageInFrameTable(FrameTable* frameTable, long page, int pid) {

}