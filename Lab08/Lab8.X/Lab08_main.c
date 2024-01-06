/*
 * File:   Player.c
 * Author: Kevin Truong (1834510)
 * Collaborated With: N/A
 * 
 */
// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
//CSE013E Support Library
#include "UNIXBOARD.h"

// User libraries
#include "Game.h"
#include "Player.h"


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****
#define BLANK_EXIT 0b0000
// **** Declare any function prototypes here ****

int main() {



    /******************************** Your custom code goes below here ********************************/
    printf("Welcome to ktruon13's RPG dungeon game!\n");
    int result = GameInit();

    if (result == STANDARD_ERROR) {
        FATAL_ERROR();
    }

    char title[GAME_MAX_ROOM_DESC_LENGTH + 1];
    char description[GAME_MAX_ROOM_DESC_LENGTH + 1];
    char userInput;
    int checker;
    uint8_t exits;
    // loops while the user has not quit
    while (1) {
        GameGetCurrentRoomTitle(title);
        printf("\n***********************************************************");
        printf("\n%s\n", title);
        GameGetCurrentRoomDescription(description);
        printf("\n%s\n", description);
        printf("***********************************************************\n");
        exits = GameGetCurrentRoomExits();
        if (exits == BLANK_EXIT) {
            printf("Game Over. Enter Q to Quit");
        } else {
            printf("Avaliable Exits: ");
        }
        if (exits & GAME_ROOM_EXIT_NORTH_EXISTS) {
            printf("North ");
        }
        if (exits & GAME_ROOM_EXIT_EAST_EXISTS) {
            printf("East ");
        }
        if (exits & GAME_ROOM_EXIT_SOUTH_EXISTS) {
            printf("South ");
        }
        if (exits & GAME_ROOM_EXIT_WEST_EXISTS) {
            printf("West ");
        }

        printf("\n");

        while (1) {
            printf("\nPick an avaliable exit. (N for North, E for East, S for South, W for West, or Q to Quit): ");
            userInput = getchar();
            if (getchar() != '\n') {
                while (getchar() != '\n');
                printf("Input invalid, please try again!\n");
                continue;
            }
            
            switch (toupper(userInput)) {
                case 'N':
                    checker = GameGoNorth();
                    if (checker == STANDARD_ERROR) {
                        printf("Cannot exit North. Try a different exit!\n");
                    }
                    break;
                case 'E':
                    checker = GameGoEast();
                    if (checker == STANDARD_ERROR) {
                        printf("Cannot exit East. Try a different exit!\n");
                    }
                    break;
                case 'S':
                    checker = GameGoSouth();
                    if (checker == STANDARD_ERROR) {
                        printf("Cannot exit South. Try a different exit!\n");
                    }
                    break;
                case 'W':
                    checker = GameGoWest();
                    if (checker == STANDARD_ERROR) {
                        printf("Cannot exit West. Try a different exit!\n");
                    }
                    break;
                case 'Q':
                    printf("\nQuitting Game....\n");
                    break;
                default:
                    printf("Invalid Input. Please Try Again!\n");
                    break;
            }
            if (userInput == 'Q' || checker == SUCCESS) {
                break;
            }
        }
        if (userInput == 'Q') {
            exit(SUCCESS);
        }

    }
    /**************************************************************************************************/
}
