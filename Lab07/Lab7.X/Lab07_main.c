// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CSE13E Support Library
#include "BOARD.h"
#include "Buttons.h"
#include "Adc.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Ascii.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>
#include <stdint.h>



// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)


// **** Set any local typedefs here ****

typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, EC_FLASH
} OvenState;

typedef struct {
    OvenState state;
    //add more members to this struct
    uint16_t cookTime, timeLeft, temperature, buttonTime;
    uint8_t mode;
} OvenData;

typedef enum {
    BAKE, TOAST, BROIL
} OvenMode;

// **** Declare any datatypes here ****
static OvenData ovenData;
static uint16_t TIMER_TICK = FALSE; //Set TIMER_TICK 2 Byte event flag.
static uint8_t ADC_CHANGED = FALSE; //Set ADC_CHANGED 1 Byte event flag.
static uint16_t FREE_RUNNING_TIME = 0; //Set free running time 2 Byte counter.
static uint8_t BUTTONS_EVENT = BUTTON_EVENT_NONE; //Set Buttons 1 Byte event flag.
static uint16_t ADC_READ; //Set ADC_READ 10bit sample.
static uint8_t EDIT_TEMPERATURE = FALSE; //Set editing temperature 1 Byte event flag.
static uint16_t TICK_COUNTER = 0; //Used to check how many ticks have passed.
static uint16_t LEDS_INTERVAL; //Used to Turn off LED 2 by 1 depending on time left.
static uint16_t LEDS_REMAIN; //How many of the 8 LEDS are left.
static uint16_t PREVIOUS_TEMPERATURE; //Used to store bake temp when cycling through modes.
static char CURRENT_LED; //State of LEDS
static uint8_t FLASH = FALSE; //Used for Extra Credits




// **** Define any module-level, global, or external variables here ****
#define TICKS_PER_SECONDS 5
#define DEFAULT_TEMPERATURE 350
#define BROIL_TEMPERATURE 500
#define LED_ON 0xFF     //turn all leds on
#define LED_OFF 0x00    //clear leds
#define TRUE 1
#define FALSE 0
#define SECONDS 60  //Used for how many seconds left in the cooktime
#define DEGREES 300 //Used as a min range of temp
#define LEDS 8      //Number of LEDS

// **** Put any helper functions here ****

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData) {
    //update OLED here
    char String[75] = "";

    char ovenTopOff[8];
    sprintf(ovenTopOff, "|%s%s%s%s%s|", OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF);

    char ovenTopOn[8];
    sprintf(ovenTopOn, "|%s%s%s%s%s|", OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON);

    char ovenBotOff[8];
    sprintf(ovenBotOff, "|%s%s%s%s%s|", OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF);

    char ovenBotOn[8];
    sprintf(ovenBotOn, "|%s%s%s%s%s|", OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON);

    char ovenDashes[] = "|-----|";
    char ovenBlank[] = "|     |";


    switch (ovenData.mode) {
        case BAKE:
            if (ovenData.state != COOKING && ovenData.state != RESET_PENDING) { //If Oven is OFF

                if (EDIT_TEMPERATURE) {
                    sprintf(String, "%s Mode: Bake" //LINE1 of OLED
                            "\n%s Time: %d:%02d" //LINE2 of OLED
                            "\n%s >Temp: %d%sF" //LINE3 of OLED
                            "\n%s", //LINE4 of OLED
                            ovenTopOff, ovenBlank, ovenData.cookTime / SECONDS, ovenData.cookTime % SECONDS, ovenDashes, ovenData.temperature, DEGREE_SYMBOL, ovenBotOff);

                } else {
                    sprintf(String, "%s Mode: Bake" //LINE1 of OLED
                            "\n%s >Time: %d:%02d" //LINE2 of OLED
                            "\n%s Temp: %d%sF" //LINE3 of OLED
                            "\n%s", //LINE4 of OLED
                            ovenTopOff, ovenBlank, ovenData.cookTime / SECONDS, ovenData.cookTime % SECONDS, ovenDashes, ovenData.temperature, DEGREE_SYMBOL, ovenBotOff);

                }
            } else {
                sprintf(String, "%s Mode: Bake" //LINE1 of OLED
                        "\n%s Time: %d:%02d" //LINE2 of OLED
                        "\n%s Temp: %d%sF" //LINE3 of OLED
                        "\n%s", //LINE4 of OLED
                        ovenTopOn, ovenBlank, ovenData.timeLeft / SECONDS, ovenData.timeLeft % SECONDS, ovenDashes, ovenData.temperature, DEGREE_SYMBOL, ovenBotOn);
            }
            break;

        case TOAST:
            if (ovenData.state != COOKING && ovenData.state != RESET_PENDING) {
                sprintf(String, "%s Mode: Toast" //LINE1 of OLED
                        "\n%s Time: %d:%02d" //LINE2 of OLED
                        "\n%s" //LINE3 of OLED
                        "\n%s", //LINE4 of OLED
                        ovenTopOff, ovenBlank, ovenData.cookTime / SECONDS, ovenData.cookTime % SECONDS, ovenDashes, ovenBotOff);
            } else {
                sprintf(String, "%s Mode: Toast" //LINE1 of OLED
                        "\n%s Time: %d:%02d" //LINE2 of OLED
                        "\n%s" //LINE3 of OLED
                        "\n%s", //LINE4 of OLED
                        ovenTopOff, ovenBlank, ovenData.timeLeft / SECONDS, ovenData.timeLeft % SECONDS, ovenDashes, ovenBotOff);
            }
            break;

        case BROIL:
            if (ovenData.state != COOKING && ovenData.state != RESET_PENDING) {
                sprintf(String, "%s Mode: Broil" //LINE1 of OLED
                        "\n%s Time: %d:%02d" //LINE2 of OLED
                        "\n%s Temp: %d%sF" //LINE3 of OLED
                        "\n%s", //LINE4 of OLED
                        ovenTopOff, ovenBlank, ovenData.cookTime / SECONDS, ovenData.cookTime % SECONDS, ovenDashes, ovenData.temperature, DEGREE_SYMBOL, ovenBotOff);
            } else {
                sprintf(String, "%s Mode: Broil" //LINE1 of OLED
                        "\n%s Time: %d:%02d" //LINE2 of OLED
                        "\n%s Temp: %d%sF" //LINE3 of OLED
                        "\n%s", //LINE4 of OLED
                        ovenTopOff, ovenBlank, ovenData.timeLeft / SECONDS, ovenData.timeLeft % SECONDS, ovenDashes, ovenData.temperature, DEGREE_SYMBOL, ovenBotOff);
            }
            break;

    }
    OledClear(OLED_COLOR_BLACK);
    OledDrawString(String);
    if (ovenData.state == EC_FLASH) {
        if (FLASH) {
            OledSetDisplayNormal();
        } else {
            OledSetDisplayInverted();
        }
    }
    OledUpdate();
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void) {
    //write your SM logic here.
    switch (ovenData.state) {
        case SETUP:
            if (ADC_CHANGED) {
                ADC_READ = AdcRead();
                ADC_READ = (ADC_READ & 0x03FC) >> 2;
                if (ovenData.mode == BAKE && EDIT_TEMPERATURE) {
                    ovenData.temperature = ADC_READ + DEGREES;
                } else {
                    ovenData.cookTime = ADC_READ + 1;
                    ovenData.timeLeft = ovenData.cookTime;
                }
                updateOvenOLED(ovenData);
            }
            if (BUTTONS_EVENT & BUTTON_EVENT_3DOWN) {
                ovenData.buttonTime = FREE_RUNNING_TIME;
                ovenData.state = SELECTOR_CHANGE_PENDING;
            }

            if (BUTTONS_EVENT & BUTTON_EVENT_4DOWN) {
                ovenData.state = COOKING;
                ovenData.buttonTime = FREE_RUNNING_TIME;
                updateOvenOLED(ovenData);
                LEDS_SET(LED_ON);
                LEDS_INTERVAL = (ovenData.cookTime * TICKS_PER_SECONDS) / LEDS;
                LEDS_REMAIN = (ovenData.cookTime * TICKS_PER_SECONDS) % LEDS;
                TICK_COUNTER = 0;
            }
            break;

        case SELECTOR_CHANGE_PENDING:
            if (BUTTONS_EVENT & BUTTON_EVENT_3UP) {
                if (FREE_RUNNING_TIME - ovenData.buttonTime >= TICKS_PER_SECONDS) {
                    if (ovenData.mode == BAKE) {
                        if (EDIT_TEMPERATURE) {
                            EDIT_TEMPERATURE = FALSE;
                        } else {
                            EDIT_TEMPERATURE = TRUE;
                        }
                    }
                    updateOvenOLED(ovenData);
                    ovenData.state = SETUP;
                } else {
                    if (ovenData.mode == BROIL) {
                        ovenData.mode = BAKE;
                    } else {
                        ovenData.mode++;
                    }
                    if (ovenData.mode == BROIL) {
                        PREVIOUS_TEMPERATURE = ovenData.temperature;
                        ovenData.temperature = BROIL_TEMPERATURE;
                    } else if (ovenData.mode == BAKE) {
                        ovenData.temperature = PREVIOUS_TEMPERATURE;
                    }
                    updateOvenOLED(ovenData);
                    ovenData.state = SETUP;
                }
            }
            break;

        case COOKING:
            if (TIMER_TICK) {
                TICK_COUNTER++;
                if ((LEDS_REMAIN > 0 && TICK_COUNTER == LEDS_INTERVAL + 1) || (LEDS_REMAIN == 0 && TICK_COUNTER == LEDS_INTERVAL)) {
                    CURRENT_LED = LEDS_GET();
                    TICK_COUNTER = 0;
                    LEDS_REMAIN--;
                    LEDS_SET(CURRENT_LED << 1);
                }
                if (ovenData.timeLeft == 0) {
                    ovenData.timeLeft = ovenData.cookTime;
                    ovenData.state = EC_FLASH;
                    updateOvenOLED(ovenData);
                    LEDS_SET(LED_OFF);
                    break;
                }
                if ((FREE_RUNNING_TIME - ovenData.buttonTime) % 5 == 0) {
                    ovenData.timeLeft--;
                    updateOvenOLED(ovenData);
                }
            }
            if (BUTTONS_EVENT & BUTTON_EVENT_4DOWN) {
                ovenData.state = RESET_PENDING;
                ovenData.buttonTime = FREE_RUNNING_TIME;
            }
            break;

        case RESET_PENDING:
            if (TIMER_TICK) {
                TICK_COUNTER++;
                if ((LEDS_REMAIN > 0 && TICK_COUNTER == LEDS_INTERVAL + 1) || (LEDS_REMAIN == 0 && TICK_COUNTER == LEDS_INTERVAL)) {
                    CURRENT_LED = LEDS_GET();
                    TICK_COUNTER = 0;
                    LEDS_REMAIN--;
                    LEDS_SET(CURRENT_LED << 1);
                }
                if ((FREE_RUNNING_TIME - ovenData.buttonTime) % 5 == 0) {
                    if (ovenData.timeLeft) {
                        ovenData.timeLeft--;
                        updateOvenOLED(ovenData);
                    }
                }
                if (FREE_RUNNING_TIME - ovenData.buttonTime >= TICKS_PER_SECONDS) {
                    ovenData.timeLeft = ovenData.cookTime;
                    ovenData.state = SETUP;
                    updateOvenOLED(ovenData);
                    LEDS_SET(LED_OFF);
                    break;
                }
            }
            if ((BUTTONS_EVENT & BUTTON_EVENT_4UP) && (FREE_RUNNING_TIME - ovenData.buttonTime < TICKS_PER_SECONDS)) {
                ovenData.state = COOKING;
            }
            break;
        case EC_FLASH:
            if (TIMER_TICK) {
                if (FLASH) {
                    FLASH = FALSE;
                } else {
                    FLASH = TRUE;
                }
                updateOvenOLED(ovenData);
            }
            if (BUTTONS_EVENT & BUTTON_EVENT_4UP) {
                FLASH = TRUE;
                updateOvenOLED(ovenData);
                ovenData.state = SETUP;
                updateOvenOLED(ovenData);
            }
            break;
    }
}

int main() {
    BOARD_Init();

    //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>

    printf("Welcome to ktruon13's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here
    AdcInit();
    ButtonsInit();
    LEDS_INIT();
    OledInit();
    ovenData.buttonTime = 0;
    ovenData.cookTime = 1;
    ovenData.timeLeft = 1;
    ovenData.temperature = DEFAULT_TEMPERATURE;
    ovenData.state = SETUP;
    ovenData.mode = BAKE;
    while (1) {
        // Add main loop code here:
        // check for events
        // on event, run runOvenSM()
        // clear event flags
        if (BUTTONS_EVENT != BUTTON_EVENT_NONE || ADC_CHANGED || TIMER_TICK) {
            runOvenSM();
            BUTTONS_EVENT = BUTTON_EVENT_NONE;
            ADC_CHANGED = FALSE;
            TIMER_TICK = FALSE;
        }
    };
}

/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void) {
    // Clear the interrupt flag.

    IFS0CLR = 1 << 12;

    //add event-checking code here
    TIMER_TICK = TRUE;
    FREE_RUNNING_TIME++;
}

/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //add event-checking code here
    ADC_CHANGED = AdcChanged();
    BUTTONS_EVENT = ButtonsCheckEvents();
}