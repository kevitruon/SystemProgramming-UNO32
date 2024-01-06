/*
 * File:   Player.c
 * Author: Kevin Truong (1834510)
 * Collaborated With: N/A
 * 
 */

#include "UNIXBOARD.h"
#include <stdint.h>
#include "Player.h"
#include <stdio.h>

static int Inventory[INVENTORY_SIZE];
static int currentItemIndex = 0;

/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item) {
    if (currentItemIndex < INVENTORY_SIZE) { //If CurrentItemIndex in range from 0-MaxInventorySize, then set to item.
        Inventory[currentItemIndex] = item;
        return SUCCESS; //If passes condition return success, else return error.
    }
    return STANDARD_ERROR;
}

/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item) {
    int scan; //Will be used to iterate through Inventory and look for item.
    for (scan = 0; scan < currentItemIndex; scan++) {
        if (Inventory[scan] == item) {
            return SUCCESS;
        }
    }
    return STANDARD_ERROR; //If item not found return error.
}