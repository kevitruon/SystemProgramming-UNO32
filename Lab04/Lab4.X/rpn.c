//  ****************************************************************************
//  Kevin Truong (1834510)
//  ktruon13@ucsc.edu
//  Collaborated With: N/A
//  ****************************************************************************

// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//  ECE13 Support Library
#include "BOARD.h"

//  User library
#include "stack.h"
#include "rpn.h"

/* RPN_Evaluate() parses and evaluates a string that contains 
 * a valid Reverse Polish Notation string (no newlines!)  
 * @param:  rpn_string - a string in polish notation.  Tokens must be either 
 *          arithmetic operators or numbers.
 * @param:  result - a pointer to a double that will be modified to contain
 *          the return value of the rpn expression.
 * @return: error - if the rpn expression is invalid, 
 *          an appropriate rpn_error value is returned.
 * 
 * RPN_Evaluate supports the following basic arithmetic operations:
 *   + : addition
 *   - : subtraction
 *   * : multiplication
 *   / : division
 * Numeric tokens can be positive or negative, and can be integers or 
 * decimal floats.  RPN_Evaluate should be able to handle strings of 
 * at least 255 length.
 * */
int RPN_Evaluate(char * rpn_string, double * result) {
    char* token = strtok(rpn_string, " "); //Gets first token
    char zero = '0', nine = '9', neg = '-', dec = '.'; //Used to check token
    double first, second; //Initialize token pointers and operand
    struct Stack stack = {}; //Initialize empty stack
    StackInit(&stack);
    int minStackLength = 2; //Used to check if stack has two values

    while (token != NULL) {
        //Check if token is a valid number.
        int decCount = 0, intCount = 0, notDigit = 0;
        for (int i = 0; i < strlen(token); i++) {
            if (!(token[i] >= zero && token[i] <= nine)) { //If not a number(0-9).
                if (!(token[i] == neg && i == 0) && !(token[i] == dec && decCount == 0)) { //If not a '.' or '-'
                    notDigit++; //False for invalid token.
                }
            } else {
                intCount++;
            }
        }
        if (!(intCount >= 1)) {
            notDigit++;
        }

        //Check if token is an operator if it wasn't a valid number
        if (notDigit != 0) {
            if (strlen(token) != 1) { //If token is not a char.
                return RPN_ERROR_INVALID_TOKEN; //Raise error, Invalid Token.
            }
            //Do Operation!!!!
            switch (token[0]) {
                case '+':
                    if (StackGetSize(&stack) >= minStackLength) {
                        StackPop(&stack, &first);
                        StackPop(&stack, &second);
                        StackPush(&stack, first + second);
                    } else {
                        return RPN_ERROR_STACK_UNDERFLOW;
                    }
                    break;
                case '-':
                    if (StackGetSize(&stack) >= minStackLength) {
                        StackPop(&stack, &first);
                        StackPop(&stack, &second);
                        StackPush(&stack, second - first);
                    } else {
                        return RPN_ERROR_STACK_UNDERFLOW;
                    }
                    break;
                case '*':
                    if (StackGetSize(&stack) >= minStackLength) {
                        StackPop(&stack, &first);
                        StackPop(&stack, &second);
                        StackPush(&stack, first * second);
                    } else {
                        return RPN_ERROR_STACK_UNDERFLOW;
                    }
                    break;
                case '/':
                    if (StackGetSize(&stack) >= minStackLength) {
                        StackPop(&stack, &first);
                        StackPop(&stack, &second);
                        if (first == 0) { //Check if we are going to divide by zero.
                            return RPN_ERROR_DIVIDE_BY_ZERO;
                        }
                        StackPush(&stack, second / first);
                    } else {
                        return RPN_ERROR_STACK_UNDERFLOW;
                    }
                    break;
                default: //If not a valid operator, raise error invalid token.
                    return RPN_ERROR_INVALID_TOKEN;
            }
        } else { //Else Token is a Valid Number. Push Value into Stack
            if (StackIsFull(&stack)) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
            StackPush(&stack, atof(token)); //Convert String into a double. Then Push.
        }
        token = strtok(NULL, " "); //Go to next token
    }

    if (StackGetSize(&stack) > 1) {
        return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
    } else if (StackGetSize(&stack) < 1) {
        return RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
    }

    StackPop(&stack, result);
    return RPN_NO_ERROR;
}

/**
 * This function should read through an array of characters, checking for backspace characters. 
 * When it encounters a backspace character, it eliminates the backspace, the preceeding character
 * (if such a character exists), and shifts all subsequent characters as appropriate.
 * @param string_to_modify The string that will be processed for backspaces. 
 *        This string is modified "in place", so it is both an input and an output to the function.
 * @return Returns the size of the resulting string in "string_to_modify".
 *
 * ProcessBackspaces() should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings of at least 255 length.
 * 
 * */
int ProcessBackspaces(char *rpn_sentence) {
    //delOnce, delete once. delTwice, delete twice. flag use to track backspace. cLen, current length of string.
    int delOnce = 1, delTwice = 2, flag = 0, cLen = strlen(rpn_sentence), iter = 0;
    while (iter < cLen) { //iterate through string
        if (rpn_sentence[iter] == '\b') { //if iteration equals backspace
            int i = 0;
            if (iter != 0) { //Making sure there is something to delete
                for (i = 0; i + flag < cLen; i++) {
                    if (i == iter - delOnce) {
                        flag = delTwice;
                    }
                    rpn_sentence[i] = rpn_sentence[i + flag];
                }
                flag = 0;
                cLen = cLen - delTwice;
                iter--;
                rpn_sentence[i] = '\0'; //turn iteration into NULL in the string
            } else { //else there is nothing to delete.
                for (i = 1; i < cLen; i++) {
                    rpn_sentence[i - delOnce] = rpn_sentence[i];
                }
                rpn_sentence[i - delOnce] = '\0'; //turn iteration into NULL in the string
                cLen--;
            }
        } else {
            iter++;
        }

    }
    return strlen(rpn_sentence); //return length of string
}