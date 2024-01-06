// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "stack.h"

int main() {
    BOARD_Init();

    printf("\n###### Beginning ktruon13's stack test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!

    printf("Does StackInit() set the currentItemIndex appropriately?\n");
    struct Stack stacktest = {};
    StackInit(&stacktest);
    if (stacktest.currentItemIndex == -1) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    double q;
    printf("Does StackPush() push value appropriately?\n");
    StackPush(&stacktest, 234.0);
    if (stacktest.currentItemIndex > -1) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    printf("Does StackPop() pop value appropriately?\n");
    StackPop(&stacktest, &q);
    if (StackIsEmpty(&stacktest)) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    printf("Does StackIsEmpty() check if stack is empty?\n");
    if (StackIsEmpty(&stacktest)) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }
    
    struct Stack stacktest2 = {};
    StackInit(&stacktest2);
    for (int i =0; i<STACK_SIZE -1;i++){
        StackPush(&stacktest2,i);
    }
    stacktest2.currentItemIndex++; //Increment the pointer to the next spot in the stack.
    stacktest2.stackItems[stacktest2.currentItemIndex] = 20.0; //Push value to stack.
    printf("Does StackIsFull() check if stack is Full?\n");
    if (StackIsFull(&stacktest2)) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }
    printf("%d", StackGetSize(&stacktest2));

    BOARD_End();
    //test stackInit:
    while (1);
    return 0;
}


