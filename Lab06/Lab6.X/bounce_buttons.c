/*
 * File:   bounce_buttons.c
 * Author: Kevin Truong (1834510)
 * Collaborated With: N/A
 * 
 */
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"
#include "Leds_Lab06.h"

// **** Set macros and preprocessor directives ****

enum LEDRegisters { //Splitting the Led Pattern to Eight Different Sections.
    LED1 = 0x03,    //Two Leftmost LEDs
    LED2 = 0x0C,    //Two InnerLeft LEDS
    LED3 = 0x30,    //Two InnerRight LEDS
    LED4 = 0xC0     //Two Rightmost LEDs
};
// **** Declare any datatypes here ****
static uint8_t event;
// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to ktruon13's lab6 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);
    LEDS_INIT();    //Initialize LEDS and Buttons
    ButtonsInit();
    while (1) {
        if (event != BUTTON_EVENT_NONE) {
            char led = LEDS_GET();
            uint8_t state = SWITCH_STATES();
            if ((state & SWITCH_STATE_SW1)&&(event & BUTTON_EVENT_1UP)) {   //Two Leftmost LEDs
                led ^= (LED1);
            } else if (!(state & SWITCH_STATE_SW1)&&(event & BUTTON_EVENT_1DOWN)) {
                led ^= (LED1);
            }

            if ((state & SWITCH_STATE_SW2)&&(event & BUTTON_EVENT_2UP)) {
                led ^= (LED2);
            } else if (!(state & SWITCH_STATE_SW2)&&(event & BUTTON_EVENT_2DOWN)) { //Two InnerLeft LEDS
                led ^= (LED2);
            }

            if ((state & SWITCH_STATE_SW3)&&(event & BUTTON_EVENT_3UP)) {       //Two InnerRight LEDS
                led ^= (LED3);
            } else if (!(state & SWITCH_STATE_SW3)&&(event & BUTTON_EVENT_3DOWN)) {
                led ^= (LED3);
            }

            if ((state & SWITCH_STATE_SW4)&&(event & BUTTON_EVENT_4UP)) {       //Two Rightmost LEDs
                led ^= (LED4);
            } else if (!(state & SWITCH_STATE_SW4)&&(event & BUTTON_EVENT_4DOWN)) {
                led ^= (LED4);
            }

            LEDS_SET(led);
            event = BUTTON_EVENT_NONE;
        }
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void) {
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    event = ButtonsCheckEvents();

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}