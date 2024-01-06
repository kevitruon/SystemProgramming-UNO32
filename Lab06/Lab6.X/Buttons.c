/*
 * File:   Buttons.c
 * Author: Kevin Truong (1834510)
 * Collaborated With: N/A
 * 
 */
#include <stdint.h>
#include <genericTypeDefs.h>
#include "BOARD.h"
#include "Buttons.h"
static uint8_t debounce;
static uint8_t previous;

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified, so that this library does not interfere with other libraries.
 */
void ButtonsInit(void) {
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
    debounce = 0;
    previous = 0x00;
}

/**
 * ButtonsCheckEvents function checks the current button states and returns
 *  any events that have occured since its last
 * call.  This function should be called repeatedly in a Timer ISR, though it can
 * be called in main() during testing.
 *
 * In normal use, this function should only be used after ButtonsInit().
 * 
 * This function should assume that the buttons start in an off state with 
 * value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, 
 * BUTTONS_EVENT_NONE should be returned. 
 * 
 * @return  Each bit of the return value corresponds to one ButtonEvent flag,
 *          as described in Buttons.h.  If no events are detected, BUTTONS_EVENT_NONE is returned.
 * 
 * Note that more than one event can occur simultaneously, though this situation is rare.
 * To handle this, the output should be a bitwise OR of all applicable event flags. 
 * For example, if button 1 was released at the same time that button 2 was pressed,
 * this function should return (BUTTON_EVENT_1UP | BUTTON_EVENT_2DOWN) 
 * 
 */
uint8_t ButtonsCheckEvents(void) {
    uint8_t button = BUTTON_EVENT_NONE;
    uint8_t status = BUTTON_STATES();
    if (debounce > 0) {
        debounce--;
        return button;
    }
    if (status == previous) {
        return BUTTON_EVENT_NONE;
    }

    if ((status & BUTTON_STATE_1) && ((previous & BUTTON_STATE_1) == 0x00)) {
        button |= BUTTON_EVENT_1DOWN;
    } else if (!(status & BUTTON_STATE_1) && previous & BUTTON_STATE_1) {
        button |= BUTTON_EVENT_1UP;
    }
    if ((status & BUTTON_STATE_2) && ((previous & BUTTON_STATE_2) == 0x00)) {
        button |= BUTTON_EVENT_2DOWN;
    } else if (!(status & BUTTON_STATE_2) && previous & BUTTON_STATE_2) {
        button |= BUTTON_EVENT_2UP;
    }
    if ((status & BUTTON_STATE_3) && ((previous & BUTTON_STATE_3) == 0x00)) {
        button |= BUTTON_EVENT_3DOWN;
    } else if (!(status & BUTTON_STATE_3) && previous & BUTTON_STATE_3) {
        button |= BUTTON_EVENT_3UP;
    }
    if ((status & BUTTON_STATE_4) && ((previous & BUTTON_STATE_4) == 0x00)) {
        button |= BUTTON_EVENT_4DOWN;
    } else if (!(status & BUTTON_STATE_4) && previous & BUTTON_STATE_4) {
        button |= BUTTON_EVENT_4UP;
    }
    debounce = BUTTONS_DEBOUNCE_PERIOD;
    previous = status;
    return button;
}
