// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "rpn.h"

// **** Set macros and preprocessor directives ****
// The lab calls for 60 characters of user input
#define MAX_INPUT_LENGTH 60
#define MAX_INPUT_ERROR 7

int main() {
    BOARD_Init();

    //What follows is starter code.  You will need to modify it!

    char rpn_sentence[MAX_INPUT_LENGTH + 2];
    double result;
    int errorVal = 0;
    char printString[MAX_INPUT_LENGTH];
    printf("Welcome to ktruon13'S RPN calculator.  Compiled on %s %s\n", __DATE__, __TIME__);
    while (1) {
        errorVal = 0;

        printf("Enter floats and + - / * in RPN format:\n");

        fgets(rpn_sentence, sizeof (rpn_sentence), stdin);
        for (int i = 0; i < strlen(rpn_sentence); i++) {
            if (rpn_sentence[i] == '\n') { //if newline, turn it into NULL.
                rpn_sentence[i] = '\0';
            }
        }
        if (strlen(rpn_sentence) > MAX_INPUT_LENGTH && rpn_sentence[MAX_INPUT_LENGTH] != '\0') {
            rpn_sentence[MAX_INPUT_LENGTH] = '\0';
            errorVal = MAX_INPUT_ERROR;
            for (int i = 0; i < strlen(rpn_sentence); i++) {
                rpn_sentence[i] = '\0';
            }
            while (getchar() != '\n');
        }
        if(errorVal!=MAX_INPUT_ERROR){
            ProcessBackspaces(rpn_sentence);
            errorVal = RPN_Evaluate(rpn_sentence, &result);
        }
        
        switch(errorVal){
            case 1:
                sprintf(printString, "\tERROR: No more room in stack.\n");
                break;
            case 2:
                sprintf(printString, "\tERROR: No enough operands before operator.\n");
                break;
            case 3:
                sprintf(printString, "\tERROR: Invalid character in RPN string.\n");
                break;
            case 4:
                sprintf(printString, "\tERROR: Dividing by zero .\n");
                break;
            case 5:
                sprintf(printString, "\tERROR: Empty Stack.\n");
                break;
            case 6:
                sprintf(printString, "\tERROR: Too many items in stack.\n");
                break;
            case 7:
                sprintf(printString, "\tERROR: Input exceeds 60 characters.\n");
                break;
            default:
                sprintf(printString, "\tResult = %lf\n", result);
                break;
        }
        printf("%s\n", printString);
    }

    while (1);
}
