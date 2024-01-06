/*
 * File:   MessageTest.c
 * Author: Kevin Truong (1834510)
 * ktruon13@ucsc.edu
 * Collaborated With: N/A
 * 
 */
#include "xc.h"
#include <stdio.h>
#include "Message.h"
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include "OledDriver.h"
#include "Oled.h"
#include "Ascii.h"

static BB_Event testEvent;
static Message testMessage;
static uint8_t checksum;
static char *payload;
static char *checkString;
static char message[MESSAGE_MAX_LEN];
static char *message2;
static int result;

int main() {

    int count = 0;
    printf("\nWelcome to ktruon13's Message.c Test Harness\n");
    printf("\nTesting Message_CalulateChecksum()\n");
    payload = "CHA,1";
    checksum = Message_CalculateChecksum(payload);
    if (checksum == 0x57) {
        count++;
    }

    payload = "SHO,1,2";
    checksum = Message_CalculateChecksum(payload);
    if (checksum == 0x57) {
        count++;
    }

    payload = "RES,1,2,3";
    checksum = Message_CalculateChecksum(payload);
    if (checksum == 0x58) {
        count++;
    }
    printf("Message_CalculateChecksum(): PASSED: %d/3\n", count);
    count = 0;
    printf("\nTesting Message_ParseMessage():\n");
    payload = "CHA,1";
    checkString = "57";
    result = Message_ParseMessage(payload, checkString, &testEvent);
    if (result && testEvent.type == BB_EVENT_CHA_RECEIVED && testEvent.param0 == 1) {
        count++;
    }

    payload = "SHO,1,2";
    checkString = "57";
    result = Message_ParseMessage(payload, checkString, &testEvent);
    if (result && testEvent.type == BB_EVENT_SHO_RECEIVED && testEvent.param0 == 1 && testEvent.param1 == 2) {
        count++;
    }

    payload = "RES,1,2,3";
    checkString = "58";
    result = Message_ParseMessage(payload, checkString, &testEvent);
    if (result && testEvent.type == BB_EVENT_RES_RECEIVED && testEvent.param0 == 1 && testEvent.param1 == 2 && testEvent.param2 == 3) {
        count++;
    }
    checkString = "46";
    result = Message_ParseMessage(payload, checkString, &testEvent);
    if (!result && testEvent.type == BB_EVENT_ERROR) {
        count++;
    }

    payload = "CHA,1,2";
    checkString = "49";
    result = Message_ParseMessage(payload, checkString, &testEvent);
    if (!result && testEvent.type == BB_EVENT_ERROR) {
        count++;
    }

    payload = "1";
    checkString = "31";
    result = Message_ParseMessage(payload, checkString, &testEvent);
    if (!result && testEvent.type == BB_EVENT_ERROR) {
        count++;
    }
    printf("Message_ParseMessage(): PASSED: %d/6\n", count);
    count = 0;

    // testing message encode
    printf("\nTesting Message_Encode():\n");

    testMessage.type = MESSAGE_CHA;
    testMessage.param0 = 1;
    Message_Encode(message, testMessage);
    if (strcmp(message, "$CHA,1*57\n") == 0) {
        count++;
    }

    testMessage.type = MESSAGE_SHO;
    testMessage.param0 = 1;
    testMessage.param1 = 2;
    Message_Encode(message, testMessage);
    if (strcmp(message, "$SHO,1,2*57\n") == 0) {
        count++;
    }

    testMessage.type = MESSAGE_RES;
    testMessage.param0 = 1;
    testMessage.param1 = 2;
    testMessage.param2 = 3;
    Message_Encode(message, testMessage);
    if (strcmp(message, "$RES,1,2,3*58\n") == 0) {
        count++;
    }
    printf("Message_Encode(): PASSED: %d/3\n", count);
    count = 0;

    printf("\nTesting Message_Decode():\n");

    // each test will send the char one by one until its done
    int iter = 0;

    message2 = "$CHA,1*57\n";
    for (iter = 0; iter < strlen(message2); iter++) {
        Message_Decode(message2[iter], &testEvent);
    }
    if (testEvent.type == BB_EVENT_CHA_RECEIVED && testEvent.param0 == 1) {
        count++;
    }

    message2 = "$SHO,1,2*57\n";
    for (iter = 0; iter < strlen(message2); iter++) {
        Message_Decode(message2[iter], &testEvent);
    }
    if (testEvent.type == BB_EVENT_SHO_RECEIVED && testEvent.param0 == 1 && testEvent.param1 == 2) {
        count++;
    }

    message2 = "$RES,1,2,3*58\n";
    for (iter = 0; iter < strlen(message2); iter++) {
        Message_Decode(message2[iter], &testEvent);
    }
    if (testEvent.type == BB_EVENT_RES_RECEIVED && testEvent.param0 == 1 && testEvent.param1 == 2 && testEvent.param2 == 3) {
        count++;
    }

    // below are tests for error, such as invalid message
    message2 = "RES,1,2,3*58\n";
    for (iter = 0; iter < strlen(message2); iter++) {
        Message_Decode(message2[iter], &testEvent);
    }
    if (testEvent.type == BB_EVENT_ERROR) {
        count++;
    }

    message2 = "RES";
    for (iter = 0; iter < strlen(message2); iter++) {
        Message_Decode(message2[iter], &testEvent);
    }
    if (testEvent.type == BB_EVENT_ERROR) {
        count++;
    }
    printf("Message_Decode(): PASSED: %d/5\n", count);
    printf("\nDone Testing");
    while (1);
}