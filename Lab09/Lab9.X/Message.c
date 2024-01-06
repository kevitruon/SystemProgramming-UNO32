/*
 * File:   Message.c
 * Author: Kevin Truong (1834510)
 * ktruon13@ucsc.edu
 * Collaborated With: N/A
 * 
 */

//Libraries
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "BattleBoats.h"
#include "Message.h"
#include "BOARD.h"

typedef enum {
    WAITING,
    RECORDING_PAYLOAD,
    RECORDING_CHECKSUM,
} DecodingState;

#define START_DELIM '$'
#define CHECKSUM_DELIM '*'
#define LAST_DELIM '\n'
#define BOUND1 65
#define BOUND2 57
#define BOUND3 48
#define BOUND4 70
#define checklength 2

static DecodingState decodeState = WAITING;
static char decPayload[MESSAGE_MAX_PAYLOAD_LEN];
static char checkSumString[3];
static int count = 0;

/**
 * Given a payload string, calculate its checkSum
 * 
 * @param payload       //the string whose checkSum we wish to calculate
 * @return   //The resulting 8-bit checkSum 
 */
uint8_t Message_CalculateChecksum(const char* payload) {
    uint8_t result = 0;
    int iter = 0;
    while (iter < strlen(payload)) {
        result ^= payload[iter];
        iter++;
    }
    return result;
}

/**
 * ParseMessage() converts a message string into a BB_Event.  The payload and
 * checkSum of a message are passed into ParseMessage(), and it modifies a
 * BB_Event struct in place to reflect the contents of the message.
 * 
 * @param payload       //the payload of a message
 * @param checkSum      //the checkSum (in string form) of  a message,
 *                          should be exactly 2 chars long, plus a null char
 * @param message_event //A BB_Event which will be modified by this function.
 *                      //If the message could be parsed successfully,
 *                          message_event's type will correspond to the message type and 
 *                          its parameters will match the message's data fields.
 *                      //If the message could not be parsed,
 *                          message_events type will be BB_EVENT_ERROR
 * 
 * @return STANDARD_ERROR if:
 *              the payload does not match the checkSum
 *              the checkSum string is not two characters long
 *              the message does not match any message template
 *          SUCCESS otherwise
 * 
 * Please note!  sscanf() has a couple compiler bugs that make it a very
 * unreliable tool for implementing this function. * 
 */
int Message_ParseMessage(const char* payload, const char* checkSum_string, BB_Event * message_event) {
    message_event->param0 = 0;
    message_event->param1 = 0;
    message_event->param2 = 0;
    char copy[MESSAGE_MAX_PAYLOAD_LEN];
    strcpy(copy, payload);

    if (strlen(checkSum_string) != 2) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    uint8_t convert = strtoul(checkSum_string, NULL, 16);

    if (Message_CalculateChecksum(payload) != convert) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    char *token = strtok(copy, ",");

    int expected_tokens;
    if (strcmp(token, "CHA") == 0) {
        expected_tokens = 1;
        message_event->type = BB_EVENT_CHA_RECEIVED;
    } else if (strcmp(token, "ACC") == 0) {
        expected_tokens = 1;
        message_event->type = BB_EVENT_ACC_RECEIVED;
    } else if (strcmp(token, "SHO") == 0) {
        expected_tokens = 2;
        message_event->type = BB_EVENT_SHO_RECEIVED;
    } else if (strcmp(token, "REV") == 0) {
        expected_tokens = 1;
        message_event->type = BB_EVENT_REV_RECEIVED;
    } else if (strcmp(token, "RES") == 0) {
        expected_tokens = 3;
        message_event->type = BB_EVENT_RES_RECEIVED;
    } else {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    int iter;
    for (iter = 0; iter < expected_tokens; iter++) {
        token = strtok(NULL, ",");

        if (token == NULL) {
            message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }

        //change the token to an int and put it in the correct param
        uint16_t p = atoi(token);
        if (iter == 0) {
            message_event->param0 = p;
        } else if (iter == 1) {
            message_event->param1 = p;
        } else if (iter == 2) {
            message_event->param2 = p;
        }
    }

    token = strtok(NULL, ",");
    if (token) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }

    return SUCCESS;

}

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. 
 * 
 * The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * 
 * @param message            The character array used for storing the output. 
 *                              Must be long enough to store the entire string,
 *                              see MESSAGE_MAX_LEN.
 * @param message_to_encode  A message to encode
 * @return                   The length of the string stored into 'message_string'.
                             Return 0 if message type is MESSAGE_NONE.
 */
int Message_Encode(char *message_string, Message message_to_encode) {
    char messageTemp[MESSAGE_MAX_PAYLOAD_LEN];
    char finalStr[MESSAGE_MAX_LEN];
    uint8_t checkSum;

    switch (message_to_encode.type) {
        case MESSAGE_NONE:
            return 0;
            break;
        case MESSAGE_ACC:
            sprintf(messageTemp, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
            checkSum = Message_CalculateChecksum(messageTemp);
            sprintf(finalStr, MESSAGE_TEMPLATE, messageTemp, checkSum);
            strcpy(message_string, finalStr);
            break;
        case MESSAGE_CHA:
            sprintf(messageTemp, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
            checkSum = Message_CalculateChecksum(messageTemp);
            sprintf(finalStr, MESSAGE_TEMPLATE, messageTemp, checkSum);
            strcpy(message_string, finalStr);
            break;
        case MESSAGE_SHO:
            sprintf(messageTemp, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
            checkSum = Message_CalculateChecksum(messageTemp);
            sprintf(finalStr, MESSAGE_TEMPLATE, messageTemp, checkSum);
            strcpy(message_string, finalStr);
            break;
        case MESSAGE_REV:
            sprintf(messageTemp, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
            checkSum = Message_CalculateChecksum(messageTemp);
            sprintf(finalStr, MESSAGE_TEMPLATE, messageTemp, checkSum);
            strcpy(message_string, finalStr);
            break;
        case MESSAGE_RES:
            sprintf(messageTemp, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1, message_to_encode.param2);
            checkSum = Message_CalculateChecksum(messageTemp);
            sprintf(finalStr, MESSAGE_TEMPLATE, messageTemp, checkSum);
            strcpy(message_string, finalStr);
            break;
        case MESSAGE_ERROR:
            break;
    }

    return strlen(message_string);
}

/**
 * Message_Decode reads one character at a time.  If it detects a full NMEA message,
 * it translates that message into a BB_Event struct, which can be passed to other 
 * services.
 * 
 * @param char_in - The next character in the NMEA0183 message to be decoded.
 * @param decoded_message - a pointer to a message struct, used to "return" a message
 *                          if char_in is the last character of a valid message, 
 *                              then decoded_message
 *                              should have the appropriate message type.
 *                          if char_in is the last character of an invalid message,
 *                              then decoded_message should have an ERROR type.
 *                          otherwise, it should have type NO_EVENT.
 * @return SUCCESS if no error was detected
 *         STANDARD_ERROR if an error was detected
 * 
 * note that ANY call to Message_Decode may modify decoded_message.
 */
int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event) {
    switch (decodeState) {
        case WAITING:
            if (char_in == START_DELIM) {
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                decodeState = RECORDING_PAYLOAD;
            } else {
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_INVALID_MESSAGE_TYPE;
                return STANDARD_ERROR;
            }
            break;
        case RECORDING_PAYLOAD:
            // in the second state we add the input to a string until we receive a *
            // if the input is too long we return error
            // if there is a character that is not a number we return error
            if (count > MESSAGE_MAX_PAYLOAD_LEN) {
                count = 0;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_PAYLOAD_LEN_EXCEEDED;
                decodeState = WAITING;

                return STANDARD_ERROR;
            } else if (char_in == LAST_DELIM || char_in == START_DELIM) {
                count = 0;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_INVALID_MESSAGE_TYPE;
                decodeState = WAITING;

                return STANDARD_ERROR;
            } else if (char_in == CHECKSUM_DELIM) {
                // might need to add null here
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                decPayload[count] = '\0';
                decodeState = RECORDING_CHECKSUM;
                count = 0;
            } else {
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                decPayload[count] = char_in;
                count++;
            }
            break;
        case RECORDING_CHECKSUM:
            // we record this into a string until \n
            // if it is too long we return error
            // if there is an invalid character we return error
            if (count > checklength) {
                count = 0;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_CHECKSUM_LEN_EXCEEDED;
                decodeState = WAITING;
                return STANDARD_ERROR;
            } else if (char_in == LAST_DELIM) {
                if (count < checklength) {
                    count = 0;
                    decoded_message_event->type = BB_EVENT_ERROR;
                    decoded_message_event->param0 = BB_ERROR_CHECKSUM_LEN_INSUFFICIENT;
                    decodeState = WAITING;
                    return STANDARD_ERROR;
                } else {
                    checkSumString[count] = '\0';
                    count = 0;
                    int result = Message_ParseMessage(decPayload, checkSumString, decoded_message_event);
                    if (result == STANDARD_ERROR) {
                        decoded_message_event->type = BB_EVENT_ERROR;
                        decoded_message_event->param0 = BB_ERROR_MESSAGE_PARSE_FAILURE;
                        decodeState = WAITING;
                        return STANDARD_ERROR;
                    }
                    decodeState = WAITING;
                }
            } else if ((char_in < BOUND1 && char_in > BOUND2) || char_in < BOUND3 || char_in > BOUND4) {
                count = 0;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_BAD_CHECKSUM;
                decodeState = WAITING;
                return STANDARD_ERROR;
            } else {
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                checkSumString[count] = char_in;
                count++;
            }
            break;
    }

    return SUCCESS;
}