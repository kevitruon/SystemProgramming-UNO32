/*
 * File:   Game.c
 * Author: Kevin Truong (1834510)
 * Collaborated With: N/A
 * 
 */
#include "UNIXBOARD.h"
#include <stdint.h>
#include "Player.h"
#include "Game.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

struct Room {
    char TITLE[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    char DESC[GAME_MAX_ROOM_DESC_LENGTH + 1];
    int north;
    int south;
    int east;
    int west;
};

static struct Room currentRoom;
static char fileStr[25];
/*
 Helper Function
 */
uint8_t openFile(int roomNumber);

/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
int GameGoNorth(void) {
    if (currentRoom.north != 0x00 && openFile(currentRoom.north) == SUCCESS) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

int GameGoEast(void) {
    if (currentRoom.east != 0x00 && openFile(currentRoom.east) == SUCCESS) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

int GameGoSouth(void) {
    if (currentRoom.south != 0x00 && openFile(currentRoom.south) == SUCCESS) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

int GameGoWest(void) {
    if (currentRoom.west != 0x00 && openFile(currentRoom.west) == SUCCESS) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void) {
    uint8_t passed = openFile(STARTING_ROOM);
    if (passed == SUCCESS) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title) {
    strcpy(title, currentRoom.TITLE);
    return strlen(title);
}

/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc) {
    strcpy(desc, currentRoom.DESC);
    return strlen(desc);
}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void) {
    uint8_t exits = 0b0000;
    if (currentRoom.north) {
        exits |= 0b1000;
    }
    if (currentRoom.east) {
        exits |= 0b0100;
    }
    if (currentRoom.south) {
        exits |= 0b0010;
    }
    if (currentRoom.west) {
        exits |= 0b0001;
    }
    return exits;
}

//Helper Function
uint8_t openFile(int roomNumber) {

    sprintf(fileStr, "RoomFiles/room%d.txt", roomNumber); //Store Room Directory into fileStr.

    FILE *filePointer; //File Pointer.
    filePointer = fopen(fileStr, "r");
    if (filePointer == NULL) { //If File Doesn't Exist, Return Error.
        FATAL_ERROR();
    }
    fseek(filePointer, 3, SEEK_SET); //Use fseek after the 4th character.

    int length; //Used To Check Length of Title, Desc., Required Items, Collectable Item.
    length = fgetc(filePointer); //Check Title 
    fread(currentRoom.TITLE, length, 1, filePointer);
    currentRoom.TITLE[length] = '\0';
    if (strlen(currentRoom.TITLE) != length) {
        return STANDARD_ERROR;
    }

    int itemFlag = 1; //Used to Check for Required Items.
    length = fgetc(filePointer); //Required Item Checker.
    while (length) {
        uint8_t item = fgetc(filePointer);
        if (FindInInventory(item) == STANDARD_ERROR) {

            itemFlag = 0;
            break;
        }
        length--;
    }


    length = fgetc(filePointer); //DESC Checker.
    fread(currentRoom.DESC, length, 1, filePointer);
    currentRoom.DESC[length] = '\0';
    if (strlen(currentRoom.DESC) != length) {
        return STANDARD_ERROR;
    }

    length = fgetc(filePointer); //Collectable Item Checker
    while (length) {
        uint8_t collectItem1 = fgetc(filePointer);
        if (FindInInventory(collectItem1) == STANDARD_ERROR) {
            if (AddToInventory(collectItem1) == STANDARD_ERROR) {
                return STANDARD_ERROR;
            }
        }
        length--;
    }


    currentRoom.north = fgetc(filePointer); //Checker for North Exit.
    currentRoom.east = fgetc(filePointer); //Checker for East Exit.
    currentRoom.south = fgetc(filePointer); //Checker for South Exit.
    currentRoom.west = fgetc(filePointer); //Checker for West Exit.


    if (itemFlag == 0) {
        length = fgetc(filePointer);
        while (length) {
            uint8_t item = fgetc(filePointer);
            if (FindInInventory(item) == STANDARD_ERROR) {
                return STANDARD_ERROR;
            }
            length--;
        }
        length = fgetc(filePointer);
        fread(currentRoom.DESC, length, 1, filePointer);
        currentRoom.DESC[length] = '\0';
        length = fgetc(filePointer);
        while (length) {
            uint8_t collectItem2 = fgetc(filePointer);
            if (FindInInventory(collectItem2) == STANDARD_ERROR) { //If not in inventory, add.
                if (AddToInventory(collectItem2) == STANDARD_ERROR) {
                    return STANDARD_ERROR;
                }
            }
            length--;
        }

        currentRoom.north = fgetc(filePointer);
        currentRoom.east = fgetc(filePointer);
        currentRoom.south = fgetc(filePointer);
        currentRoom.west = fgetc(filePointer);
    }
    fclose(filePointer);
    return SUCCESS;
}
