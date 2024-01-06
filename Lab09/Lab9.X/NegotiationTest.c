/*
 * File:   NegotiationTest.c
 * Author: Kevin Truong (1834510)
 * ktruon13@ucsc.edu
 * Collaborated With: N/A
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include "Negotiation.h"
#include "Uart1.h"

int main() {

    int count = 0;
    printf("\nWelcome to ktruon13's Negotiation.c Test Harness\n");
    printf("\nTesting NegotiationHash()\n");
    
    NegotiationData secret = 3;
    NegotiationData correctHash = (secret * secret) % 0xBEEF;
    if (NegotiationHash(secret) == correctHash) {
        count++;
    }
    if (NegotiationHash(12345) == 43182) {
        count++;
    }
    if (NegotiationHash(45896) == 2311) {
        count++;
    }
    printf("NegotiationHash(): PASSED: %d/3\n",count);
    
    printf("\nTesting NegotiationVerify()\n");
    NegotiationData cheat = 50;
    count=0;
    if (NegotiationVerify(cheat, correctHash) == FALSE && NegotiationVerify(secret, correctHash) == TRUE) {
        count++;
    }
    if (NegotiationVerify(45896, 2311) == TRUE) {
        count++;
    }
    printf("NegotiationVerify(): PASSED: %d/2\n",count);

    printf("\nTesting NegotiationVerify()\n");
    count=0;
    if (NegotiateCoinFlip(secret, cheat) == HEADS) {
        count++;
    }
    if (NegotiateCoinFlip(10, 10) == TAILS) {
        count++;
    }
    printf("NegotiationCoinFlip(): PASSED: %d/2\n",count);
    
    printf("\nDone testing!\n");
    while(1);
}