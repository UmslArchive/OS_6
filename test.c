//Author: Colby Ackerman

#include "shared.h"

void testTimeDifference();
void testTimeHasPassed();
void testClockSet();
void testPcbArrayInit();
void testProcessSpawnDespawn();

//===========================================================


int main() {
    srand(time(NULL));

    printf("\nRUNNING TESTS:\n-----\n");

    //Clock tests
    testTimeDifference();
    testTimeHasPassed();
    testClockSet();

    //PCB tests
    testPcbArrayInit();
    testProcessSpawnDespawn();

    //Memory tests

    return 0;
}

//==========================================================

void testTimeDifference() {
    Clock first;
    Clock second;
    Clock result;

    //Test case of no carry
    first.seconds = 1;
    first.nanoseconds = 5;
    second.seconds = 1;
    second.nanoseconds = 1;

    //Test label
    printf("Time Difference Tests:\n");
    
    printf("1:000000005 - 1:000000001 = \n");
    result = timeDifference(&first, &second);
    printClock(&result);

    //Test case of carry
    first.seconds = 5;
    first.nanoseconds = 5;
    second.seconds = 0;
    second.nanoseconds = 10;

    printf("\n5:000000005 - 0:000000010 = \n");
    result = timeDifference(&first, &second);
    printClock(&result);

    //Test underflow (Fails)
    first.seconds = 1;
    first.nanoseconds = 0;
    second.seconds = 2;
    second.nanoseconds = 1;

    printf("\n1:000000000 - 2:000000001 = \n");
    result = timeDifference(&first, &second);
    printClock(&result);

    printf("-----\n");
}

void testTimeHasPassed() {
    Clock mainClock;
    Clock timeLimit;
    int result;

    //Test Label
    printf("Time has come tests:\n");

    //Test 1ns before
    mainClock.seconds = 0;
    mainClock.nanoseconds = 999999999;
    timeLimit.seconds = 1;
    timeLimit.nanoseconds = 0;

    result = checkIfPassedTime(&mainClock, &timeLimit);
    printf("Is 0:999999999 after or at 1:000000000?\t");
    result == 1 ? printf("YES\n") : printf("NO\n");

    //Test exact match
    mainClock.seconds = 1;
    mainClock.nanoseconds = 0;
    timeLimit.seconds = 1;
    timeLimit.nanoseconds = 0;

    result = checkIfPassedTime(&mainClock, &timeLimit);
    printf("Is 1:000000000 after or at 1:000000000?\t");
    result == 1 ? printf("YES\n") : printf("NO\n");

    //Test 1ns after
    mainClock.seconds = 1;
    mainClock.nanoseconds = 1;
    timeLimit.seconds = 1;
    timeLimit.nanoseconds = 0;

    result = checkIfPassedTime(&mainClock, &timeLimit);
    printf("Is 1:000000001 after or at 1:000000000?\t");
    result == 1 ? printf("YES\n") : printf("NO\n");

    //Test only seconds after
    mainClock.seconds = 6;
    mainClock.nanoseconds = 100;
    timeLimit.seconds = 5;
    timeLimit.nanoseconds = 100;

    result = checkIfPassedTime(&mainClock, &timeLimit);
    printf("Is 6:000000000 after or at 5:000000000?\t");
    result == 1 ? printf("YES\n") : printf("NO\n");

    //Test only seconds before
    mainClock.seconds = 4;
    mainClock.nanoseconds = 100;
    timeLimit.seconds = 5;
    timeLimit.nanoseconds = 100;

    result = checkIfPassedTime(&mainClock, &timeLimit);
    printf("Is 4:000000000 after or at 5:000000000?\t");
    result == 1 ? printf("YES\n") : printf("NO\n");

    printf("-----\n");
}

void testClockSet() {
    Clock clock;
    initClock(&clock);

    //Test label
    printf("Set clock tests:\n");

    //nanosec to0 high test
    printf("Attempting to set clock to 1:1000000000\n");
    setClock(&clock, 1, 1000000000);
    printClock(&clock);

    //Edge case
    printf("\nAttempting to set clock to 17:999999999\n");
    setClock(&clock, 17, 999999999);
    printClock(&clock);

    printf("-----\n");
}

void testPcbArrayInit() {
    PCB* pcbArray = (PCB*)malloc(MAX_CHILD_PROCESSES * sizeof(PCB));
    int i;

    //Test label
    printf("PCB Array Tests:\n");

    ossInitPcbArray(pcbArray);

    printPcbArray(pcbArray);

    fprintf(stderr, "\n");

    PCB* iterator = pcbArray;
    for(i = 0; i < 3; ++i) {
        iterator->pid = i;
        printPcb(pcbArray, i);
        iterator++;
    }

    iterator = pcbArray;
    iterator += 10;
    iterator->pid = 1000;

    fprintf(stderr, "\n");
    
    printPcbArray(pcbArray);

    //Test label
    printf("-----\n");

    free(pcbArray);
}

void testProcessSpawnDespawn() {
    PCB* pcbArray = (PCB*)malloc(MAX_CHILD_PROCESSES * sizeof(PCB));
    int i;

    ossInitPcbArray(pcbArray);

    //Test label
    printf("Process Spawn/Despawn Tests:\n");

    for(i = 0; i < 10; ++i) {
        spawnDummyProcess(pcbArray, i + 1);
    }

    removeFromPcbArray(pcbArray, 8);

    removeFromPcbArray(pcbArray, 20);

    addToPcbArray(pcbArray, 50);
    addToPcbArray(pcbArray, 500);

    printPcbArray(pcbArray);


    free(pcbArray);
}