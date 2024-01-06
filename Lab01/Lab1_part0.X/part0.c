/* 
 * File:   part0.c
 * Author: Kevin Truong
 *
 * Created on January 8, 2022, 5:39 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"

/*
 * 
 */
int main(void) 
{
    BOARD_Init();
    printf("Hello World\n");
    
    
    BOARD_End();
    while(1);
}
