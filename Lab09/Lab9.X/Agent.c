/*
 * File:   Agent.c
 * Author: Kevin Truong (1834510)
 * ktruon13@ucsc.edu
 * Collaborated With: N/A
 * 
 */

//Libraries
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "BOARD.h"
#include "Oled.h"

#include "Agent.h"
#include "Ascii.h"
#include "Message.h"
#include "BattleBoats.h"
#include "Buttons.h"
#include "CircularBuffer.h"
#include "FieldOled.h"
#include "OledDriver.h"
#include "Uart1.h"
#include "Negotiation.h"
#include "Field.h"

struct Agent {
    AgentState state;
    NegotiationData secret, theirSecret;
    NegotiationData hash;
    Field mine;
    Field their;
    Message message;
};

typedef enum {
    LOSS,
    WON
} EndGame;

static struct Agent agent;
static EndGame endGame;
static int count = 0;
static FieldOledTurn turn;
static char secret[50], theirSecret[50], hash[50];

#define RANDSIZE 0xFFFF
#define DESTROYED 0b00000000

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *   -setting the start state of the Agent SM.
 *   -setting turn counter to 0
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * It is not advised to call srand() inside of AgentInit.  
 *  */
void AgentInit(void) {
    agent.state = AGENT_STATE_START;
    agent.secret = 0;
    agent.theirSecret = 0;
    agent.hash = 0;
    count = 0;
    turn = FIELD_OLED_TURN_NONE;
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */
Message AgentRun(BB_Event event) {
    if (event.type == BB_EVENT_RESET_BUTTON) {
        AgentInit();
        OledClear(OLED_COLOR_BLACK);
        OledDrawString("START");
        OledDrawString("\n\nReady for a new game?");
        OledDrawString("\n\n\nPress BTN4 to Start");
        OledUpdate();
    }
    agent.message.type = MESSAGE_NONE;
    switch (agent.state) {
        case AGENT_STATE_START:
            if (event.type == BB_EVENT_START_BUTTON) {
                agent.secret = rand() & RANDSIZE;
                agent.hash = NegotiationHash(agent.secret);
                agent.message.type = MESSAGE_CHA;
                agent.message.param0 = agent.hash;
                FieldInit(&agent.mine, &agent.their);
                FieldAIPlaceAllBoats(&agent.mine);
                agent.state = AGENT_STATE_CHALLENGING;
            } else if (event.type == BB_EVENT_CHA_RECEIVED) {
                agent.theirSecret = rand() & RANDSIZE;
                agent.hash = event.param0;
                agent.message.type = MESSAGE_ACC;
                agent.message.param0 = agent.theirSecret;
                FieldInit(&agent.mine, &agent.their);
                FieldAIPlaceAllBoats(&agent.mine);
                agent.state = AGENT_STATE_ACCEPTING;
            }

            break;
        case AGENT_STATE_CHALLENGING:
            OledClear(OLED_COLOR_BLACK);
            if (event.type == BB_EVENT_ACC_RECEIVED) {
                agent.theirSecret = event.param0; 
                agent.message.type = MESSAGE_REV;
                agent.message.param0 = agent.secret;
                NegotiationOutcome coin = NegotiateCoinFlip(agent.secret, agent.theirSecret);
                if (coin == HEADS) {
                    agent.state = AGENT_STATE_WAITING_TO_SEND;
                } else if (coin == TAILS) {
                    agent.state = AGENT_STATE_DEFENDING;
                }
            }
            OledDrawString("CHALLENGING");
            sprintf(secret, "\n %d = A", (int) agent.secret);
            OledDrawString(secret);
            sprintf(hash, "\n\n %d = hash_a", (int) agent.hash);
            OledDrawString(hash);
            OledUpdate();
            break;
        case AGENT_STATE_ACCEPTING:
            OledClear(OLED_COLOR_BLACK);
            if (event.type == BB_EVENT_REV_RECEIVED) {
                agent.secret = event.param0;
                NegotiationOutcome coin = NegotiateCoinFlip(agent.secret, agent.theirSecret);
                if (!NegotiationVerify(agent.secret, agent.hash)) {
                    agent.state = AGENT_STATE_END_SCREEN;
                }
                if (coin == HEADS) {
                    agent.state = AGENT_STATE_DEFENDING;
                } else if (coin == TAILS) {
                    GuessData guessOut = FieldAIDecideGuess(&agent.their);
                    agent.message.type = MESSAGE_SHO;
                    agent.message.param0 = guessOut.row;
                    agent.message.param1 = guessOut.col;
                    agent.state = AGENT_STATE_ATTACKING;
                }
            }
            OledDrawString("ACCEPTING");
            sprintf(hash, "\n\n %d = hash_a", (int) agent.hash);
            OledDrawString(hash);
            sprintf(theirSecret, "\n %d = B", (int) agent.theirSecret);
            OledDrawString(theirSecret);
            OledUpdate();
            break;
        case AGENT_STATE_WAITING_TO_SEND:
            OledClear(OLED_COLOR_BLACK);
            if (event.type == BB_EVENT_MESSAGE_SENT) {
                turn = FIELD_OLED_TURN_MINE;
                count++;
                GuessData guessOut = FieldAIDecideGuess(&agent.their);
                agent.message.type = MESSAGE_SHO;
                agent.message.param0 = guessOut.row;
                agent.message.param1 = guessOut.col;
                agent.state = AGENT_STATE_ATTACKING;
            }

            FieldOledDrawScreen(&agent.mine, &agent.their, turn, count);
            break;
        case AGENT_STATE_ATTACKING:
            OledClear(OLED_COLOR_BLACK);
            if (event.type == BB_EVENT_RES_RECEIVED) {
                GuessData guessRes;
                guessRes.row = event.param0;
                guessRes.col = event.param1;
                guessRes.result = event.param2;
                FieldUpdateKnowledge(&agent.their, &guessRes);
                if (!(agent.their.hugeBoatLives) && !(agent.their.largeBoatLives) && !(agent.their.mediumBoatLives) && !(agent.their.smallBoatLives)) {
                    agent.state = AGENT_STATE_END_SCREEN;
                    endGame = WON;
                } else {
                    agent.state = AGENT_STATE_DEFENDING;
                }
            }

            FieldOledDrawScreen(&agent.mine, &agent.their, turn, count);
            break;
        case AGENT_STATE_DEFENDING:
            if (event.type == BB_EVENT_SHO_RECEIVED) {
                turn = FIELD_OLED_TURN_THEIRS;
                GuessData guessRes;
                guessRes.row = event.param0;
                guessRes.col = event.param1;
                FieldRegisterEnemyAttack(&agent.mine, &guessRes);
                agent.message.type = MESSAGE_RES;
                agent.message.param0 = guessRes.row;
                agent.message.param1 = guessRes.col;
                agent.message.param2 = guessRes.result;

                if (!(agent.mine.hugeBoatLives) && !(agent.mine.largeBoatLives) && !(agent.mine.mediumBoatLives) && !(agent.mine.smallBoatLives)) {
                    // The game has been lost
                    agent.state = AGENT_STATE_END_SCREEN;
                    endGame = LOSS;
                } else {
                    agent.state = AGENT_STATE_WAITING_TO_SEND;
                }
            }

            FieldOledDrawScreen(&agent.mine, &agent.their, turn, count);
            break;
        case AGENT_STATE_END_SCREEN:
            OledClear(OLED_COLOR_BLACK);
            if (endGame == WON) {
                OledDrawString("VICTORY!!!\n");
            } else if (endGame == LOSS) {
                OledDrawString("DEFEAT!!!\n");
            } else {
                OledDrawString("FATAL ERROR\n");
            }
            OledUpdate();
            break;
        case AGENT_STATE_SETUP_BOATS:
            break;
    }
    return agent.message;
}

/** * 
 * @return Returns the current state that AgentGetState is in.  
 * 
 * This function is very useful for testing AgentRun.
 */
AgentState AgentGetState(void) {
    return agent.state;
}

/** * 
 * @param Force the agent into the state given by AgentState
 * 
 * This function is very useful for testing AgentRun.
 */
void AgentSetState(AgentState newState) {
    agent.state = newState;
}