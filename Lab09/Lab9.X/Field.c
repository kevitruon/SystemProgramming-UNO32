/*
 * File:   Field.c
 * Author: Kevin Truong (1834510)
 * ktruon13@ucsc.edu
 * Collaborated With: N/A
 * 
 */

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include "Field.h"
#include "BOARD.h"

#define TWO 2

/**
 * This function is optional, but recommended.   It prints a representation of both
 * fields, similar to the OLED display.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     SquareStatus.
 */
void FieldPrint_UART(Field *own_field, Field * opp_field) {
    int rows, cols;
    for (rows = 0; rows < FIELD_ROWS; rows++) {
        for (cols = 0; cols < FIELD_COLS; cols++) {
            printf(" %d", opp_field->grid[rows][cols]);
        }
        printf("\n");
    }
}

/**
 * FieldInit() will initialize two passed field structs for the beginning of play.
 * Each field's grid should be filled with the appropriate SquareStatus (
 * FIELD_SQUARE_EMPTY for your own field, FIELD_SQUARE_UNKNOWN for opponent's).
 * Additionally, your opponent's field's boatLives parameters should be filled
 *  (your own field's boatLives will be filled when boats are added)
 * 
 * FieldAI_PlaceAllBoats() should NOT be called in this function.
 * 
 * @param own_field     //A field representing the agents own ships
 * @param opp_field     //A field representing the opponent's ships
 */
void FieldInit(Field *own_field, Field * opp_field) {
    int rows, cols;
    for (rows = 0; rows < FIELD_ROWS; rows++) {
        for (cols = 0; cols < FIELD_COLS; cols++) {
            opp_field->grid[rows][cols] = FIELD_SQUARE_EMPTY;
            opp_field->grid[rows][cols] = FIELD_SQUARE_UNKNOWN;
        }
    }
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;

    own_field->smallBoatLives = 0;
    own_field->mediumBoatLives = 0;
    own_field->largeBoatLives = 0;
    own_field->hugeBoatLives = 0;
}

/**
 * Retrieves the value at the specified field position.
 * @param f     //The Field being referenced
 * @param row   //The row-component of the location to retrieve
 * @param col   //The column-component of the location to retrieve
 * @return  FIELD_SQUARE_INVALID if row and col are not valid field locations
 *          Otherwise, return the status of the referenced type 
 */
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col) {
    if ((row > 0 && row < FIELD_ROWS) && (col > 0 && col < FIELD_COLS)) {
        return f->grid[row][col];
    } else {
        return FIELD_SQUARE_INVALID;
    }
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_SQUARE_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p) {
    SquareStatus old = f->grid[row][col]; //Out with the old
    f->grid[row][col] = p; //In with the new
    return old;
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. 
 * 
 * All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 * 
 * In addition, this function should update the appropriate boatLives parameter of the field.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_SQUARE_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_TYPE_SMALL);
 * }
 *
 * should result in a field like:
 *      0 1 2 3 4 5 6 7 8 9
 *     ---------------------
 *  0 [ 3 3 3 . . . 3 . . . ]
 *  1 [ 4 4 4 4 . . 3 . . . ]
 *  2 [ . . . . . . 3 . . . ]
 *  3 [ . . . . . . . . . . ]
 *  4 [ . . . . . . . . . . ]
 *  5 [ . . . . . . . . . . ]
 *     
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_SQUARE_*_BOAT values from the
 * SquareStatus enum.
 * @return SUCCESS for success, STANDARD_ERROR for failure
 */
uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type) {
    int boatLength = 0;
    SquareStatus type = FIELD_SQUARE_INVALID;
    switch (boat_type) {
        case FIELD_BOAT_TYPE_SMALL:
            boatLength = FIELD_BOAT_SIZE_SMALL;
            type = FIELD_SQUARE_SMALL_BOAT;
            break;
        case FIELD_BOAT_TYPE_MEDIUM:
            boatLength = FIELD_BOAT_SIZE_MEDIUM;
            type = FIELD_SQUARE_MEDIUM_BOAT;
            break;
        case FIELD_BOAT_TYPE_LARGE:
            boatLength = FIELD_BOAT_SIZE_LARGE;
            type = FIELD_SQUARE_LARGE_BOAT;
            break;
        case FIELD_BOAT_TYPE_HUGE:
            boatLength = FIELD_BOAT_SIZE_HUGE;
            type = FIELD_SQUARE_HUGE_BOAT;
            break;
        default:
            return STANDARD_ERROR;
    }
    int lastindex;
    int maxlen;
    if (dir == FIELD_DIR_SOUTH) {
        lastindex = row + boatLength;
        maxlen = FIELD_ROWS;
    } else {
        lastindex = col + boatLength;
        maxlen = FIELD_COLS;
    }
    if ((row >= 0) && (col >= 0) && (row < FIELD_ROWS) && (col < FIELD_COLS) && (lastindex < maxlen)) {
       
        if (dir == FIELD_DIR_SOUTH) {

            int y = row;
            for (y = row; y < (boatLength + row); y++) {
                if (own_field->grid[y][col] == FIELD_SQUARE_EMPTY) {
                    continue;
                } else {
                    return STANDARD_ERROR;
                }
            }
            for (y = row; y < (boatLength + col); y++) {
                own_field->grid[y][col] = type;
            }

        } else if (dir == FIELD_DIR_EAST) {
            
            int x = col;
            for (x = col; x < (boatLength + col); x++) {
                if (own_field->grid[row][x] == FIELD_SQUARE_EMPTY) {
                    continue;
                } else {
                    return STANDARD_ERROR;
                }
            }
            for (x = col; x < (boatLength + col); x++) {
                own_field->grid[row][x] = type;
            }
        }
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_SQUARE_HIT or FIELD_SQUARE_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The result is stored in gData->result as an
 *               output.  The result can be a RESULT_HIT, RESULT_MISS, or RESULT_***_SUNK.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess) {
    SquareStatus storedData = own_field->grid[opp_guess->row][opp_guess->col];
    switch (own_field->grid[opp_guess->row][opp_guess->col]) {
        case FIELD_BOAT_SIZE_SMALL:
            if ((own_field->smallBoatLives) > 0) {
                opp_guess->result = RESULT_HIT;
            } else {
                opp_guess->result = RESULT_SMALL_BOAT_SUNK;
            }
            own_field->grid[opp_guess->row][opp_guess->col] = RESULT_HIT;
        case FIELD_BOAT_SIZE_MEDIUM:
            if ((own_field->mediumBoatLives) > 0) {
                opp_guess->result = RESULT_HIT;
            } else {
                opp_guess->result = RESULT_MEDIUM_BOAT_SUNK;
            }
            own_field->grid[opp_guess->row][opp_guess->col] = RESULT_HIT;
        case FIELD_BOAT_SIZE_LARGE:
            if ((own_field->largeBoatLives) > 0) {
                opp_guess->result = RESULT_HIT;
            } else {
                opp_guess->result = RESULT_LARGE_BOAT_SUNK;
            }
            own_field->grid[opp_guess->row][opp_guess->col] = RESULT_HIT;
        case FIELD_BOAT_SIZE_HUGE:
            if ((own_field->hugeBoatLives) > 0) {
                opp_guess->result = RESULT_HIT;
            } else {
                opp_guess->result = RESULT_HUGE_BOAT_SUNK;
            }
            own_field->grid[opp_guess->row][opp_guess->col] = RESULT_HIT;
    }
    return storedData;
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_SQUARE_HIT at that position. If it was a miss, display a FIELD_SQUARE_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * RESULT_*_BOAT_SUNK.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess) {
    SquareStatus previousValue = opp_field->grid[own_guess->row][own_guess->col];

    if (own_guess->result == RESULT_HIT) {
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    if (own_guess->result == RESULT_MISS) {
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
    }
    if (own_guess->result == RESULT_SMALL_BOAT_SUNK) {
        opp_field->smallBoatLives = 0;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    if (own_guess->result == RESULT_MEDIUM_BOAT_SUNK) {
        opp_field->mediumBoatLives = 0;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    if (own_guess->result == RESULT_LARGE_BOAT_SUNK) {
        opp_field->largeBoatLives = 0;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    if (own_guess->result == RESULT_HUGE_BOAT_SUNK) {
        opp_field->hugeBoatLives = 0;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    return previousValue;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f){
    uint8_t shipLiving = 0;

    if (f->smallBoatLives >= 0){
        shipLiving |= FIELD_BOAT_STATUS_SMALL;
    }
    if (f->mediumBoatLives >= 0){
        shipLiving |= FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->largeBoatLives >= 0){
        shipLiving |= FIELD_BOAT_STATUS_LARGE;
    }
    if (f->hugeBoatLives >= 0){
        shipLiving |= FIELD_BOAT_STATUS_HUGE;
    }
    return shipLiving;
}


/**
 * This function is responsible for placing all four of the boats on a field.
 * 
 * @param f         //agent's own field, to be modified in place.
 * @return SUCCESS if all boats could be placed, STANDARD_ERROR otherwise.
 * 
 * This function should never fail when passed a properly initialized field!
 */
uint8_t FieldAIPlaceAllBoats(Field *own_field){
    int smallPlaced = FALSE;
    int mediumPlaced = FALSE;
    int largePlaced = FALSE;
    int hugePlaced = FALSE;

    uint8_t row;
    uint8_t col;
    uint8_t dir;

    while (hugePlaced == FALSE || hugePlaced == SUCCESS) {
        dir = rand() % TWO;
        col = rand() % FIELD_COLS;
        row = rand() % FIELD_ROWS;

        if (smallPlaced == FALSE) {
            smallPlaced = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_SMALL);
        }
        if (smallPlaced == SUCCESS && mediumPlaced == FALSE && largePlaced == FALSE && hugePlaced == FALSE) {
            mediumPlaced = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_MEDIUM);
        } else if (mediumPlaced == SUCCESS && largePlaced == FALSE && hugePlaced == FALSE) {
            largePlaced = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_LARGE);
        } else if (largePlaced == SUCCESS && hugePlaced == FALSE) {
            hugePlaced = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_HUGE);
        } else if (hugePlaced == SUCCESS) {
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;
}

/**
 * Given a field, decide the next guess.
 *
 * This function should not attempt to shoot a type which has already been guessed.
 *
 * You may wish to give this function static variables.  If so, that data should be
 * reset when FieldInit() is called.
 * 
 * @param f an opponent's field.
 * @return a GuessData struct whose row and col parameters are the coordinates of the guess.  The 
 *           result parameter is irrelevant.
 */
GuessData FieldAIDecideGuess(const Field *opp_field){
    int unfinished = TRUE;
    int col;
    int row;
    GuessData myGuess;
    while (unfinished) {
        col = rand() % FIELD_COLS;
        row = rand() % FIELD_ROWS;
        if (opp_field->grid[row][col] == FIELD_SQUARE_UNKNOWN) {
            unfinished = FALSE;
            myGuess.col = col;
            myGuess.row = row;
        }
    }
    return myGuess;
}
