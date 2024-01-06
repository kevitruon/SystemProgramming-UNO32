// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "rpn.h"

int main() {
    BOARD_Init();
    int error;

    printf("\n###### Beginning ktruon13's rpn test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    //TEST0
    char test0[] = "1 1 +";
    double result0;
    double expected0 = 2;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test0);
    error = RPN_Evaluate(test0, &result0);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result0 != expected0) {
        printf("   Failed, expected = %f , result = %f\n", expected0, result0);
    } else {
        printf("   Success!\n");
    }

    //test1
    char test1[] = "1 2 -";
    double result1;
    double expected1 = -1;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test1);
    error = RPN_Evaluate(test1, &result1);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result1 != expected1) {
        printf("   Failed, expected = %f , result = %f\n", expected1, result1);
    } else {
        printf("   Success!\n");
    }

    //test2
    char test2[] = "10 -2 /";
    double result2;
    double expected2 = -5;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test2);
    error = RPN_Evaluate(test2, &result2);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result1 != expected1) {
        printf("   Failed, expected = %f , result = %f\n", expected2, result2);
    } else {
        printf("   Success!\n");
    }

    //test3
    char test3[] = "10 0 /";
    double result3;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test3);
    error = RPN_Evaluate(test3, &result3);
    if (error) {
        printf("   Success, RPN_Evaluate produced a divide by zero error\n");
    } else {
        printf("   Failed, did not produce expected error.\n");
    }

    //test4
    char test4[] = "12 5 *";
    double result4;
    double expected4 = 60;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test4);
    error = RPN_Evaluate(test4, &result4);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result4 != expected4) {
        printf("   Failed, expected = %f , result = %f\n", expected4, result4);
    } else {
        printf("   Success!\n");
    }

    printf("Testing ProcessBackspaces:\n");
    char test_pb1[] = "123\b34";
    ProcessBackspaces(test_pb1);
    printf("result    : %s\n", test_pb1);
    printf("should be : 1234\n");
    BOARD_End();
    while (1);
}


