//  ****************************************************************************
//  Kevin Truong (1834510)
//  ktruon13@ucsc.edu
//  Collaborated With: N/A
//  ****************************************************************************

// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//ECE13 Support Library
#include "BOARD.h"
#include "stack.h"

/**
 * This function initializes the stack. For a statically allocated stack such
 * as this, this means initializing currentItemIndex to -1 and setting
 * initialized to TRUE.
 * @param:  *stack, a pointer to a stack which is modified in place.
 */
void StackInit(struct Stack *stack) {
    stack->currentItemIndex = -1; // Using -> sets the stack variables to their values
    stack->initialized = TRUE;
}

/**
 * Pushes a value onto the stack.  The stack index is incremented, and the passed value
 * is placed on top of the stack
 * @param:  *stack, a pointer to a stack, which is read and modified in place
 * @param:  value, the value to push.
 * @return: SUCCESS if the stack is modified successfully
 *          STANDARD_ERROR if the stack isn't initialized, or is full.
 * Note that SUCCESS and STANDARD_ERROR are defined in BOARD.h.
 */
int StackPush(struct Stack *stack, double value) {
    if (stack->initialized && stack->currentItemIndex < STACK_SIZE - 1) { //Check if stack is initialized and not full.
        stack->currentItemIndex++; //Increment the pointer to the next spot in the stack.
        stack->stackItems[stack->currentItemIndex] = value; //Push value to stack.
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * Pop an element off of a stack.   The popped value is passed out by reference, and the 
 * stack index is decremented.  
 * @param:  *stack, a pointer to a stack, which is read and modified in place
 * @param:  *value, pointer to a double which stores the popped value.
 * @return: SUCCESS if the stack is modified successfully
 *          STANDARD_ERROR if the stack isn't initialized, or is empty.
 *
 * NOTE: When trying to return the output into the value variable, you'll need
 * to write it with an extra asterisk, like:
 *   *value = 7.0;
 * This is because "value" is a pointer. K&R 5.1-5.2 should explain this practice of "passing by reference". 
 */
int StackPop(struct Stack *stack, double *value) {
    if (stack->initialized && stack->currentItemIndex >= 0) { //Check if stack not empty
        *value = stack->stackItems[stack->currentItemIndex]; //Save pointer to value.
        stack->currentItemIndex--; //decrement the pointer.
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * This function checks for whether the stack is empty or not. In this simple
 * case this merely checks if the currentItemIndex == -1. 
 * @param:  *stack, a pointer to a stack
 * @return: TRUE if stack is initialized and empty
 *          FALSE if the stack isn't initialized, or is not empty.
 * Note that TRUE and FALSE are defined in BOARD.h.
 */
int StackIsEmpty(const struct Stack *stack) {
    if (stack->initialized && stack->currentItemIndex < 0) {
        return TRUE;
    }
    return FALSE;
}

/**
 * This function checks for whether the stack is empty or not. For this simple
 * implementation that is merely if the currentItemIndex == STACK_SIZE - 1. 
 * @param:  *stack, a pointer to a stack
 * @return: TRUE if the stack is full and FALSE if not.
 *          Also returns FALSE if the stack isn't initialized.
 */
int StackIsFull(const struct Stack *stack) {
    if (stack->initialized && stack->currentItemIndex >= STACK_SIZE - 1) { //If stack size == max return true, else false.
        return TRUE;
    }
    return FALSE;
}

/**
 * Returns the current size of the stack in terms of how many active elements
 * are in it. 
 * @param:  *stack, a pointer to a stack
 * @return: SIZE_ERROR for uninitialized stacks, 
 *          the number of elements in the stack for initialized stacks, otherwise. 
 *          Note that SIZE_ERROR is declared in the BOARD.h header file.
 */
int StackGetSize(const struct Stack *stack) {
    if (stack->initialized) {
        return stack->currentItemIndex + 1; //Add one since index starts from 0.
    }
    return SIZE_ERROR;
}