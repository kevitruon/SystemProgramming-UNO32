/*
 * File:   AgentTest.c
 * Author: Kevin Truong (1834510)
 * ktruon13@ucsc.edu
 * Collaborated With: N/A
 * 
 */

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include "Agent.h"
#include "BattleBoats.h"

int main() {
    printf("Welcome to ktruon13's Test Harness for Agent.c\n");
    printf("Testing AgentSetState() and AgentGetState...\n");
    int count = 0;
    AgentSetState(AGENT_STATE_START);
    if(AgentGetState() == AGENT_STATE_START){
        count++;
    }
    AgentSetState(AGENT_STATE_CHALLENGING);
    if(AgentGetState() == AGENT_STATE_CHALLENGING){
        count++;
    }
    AgentSetState(AGENT_STATE_ACCEPTING);
    if(AgentGetState() == AGENT_STATE_ACCEPTING){
        count++;
    }
    AgentSetState(AGENT_STATE_ATTACKING);
    if(AgentGetState() == AGENT_STATE_ATTACKING){
        count++;
    }
    AgentSetState(AGENT_STATE_DEFENDING);
    if(AgentGetState() == AGENT_STATE_DEFENDING){
        count++;
    }
    AgentSetState(AGENT_STATE_WAITING_TO_SEND);
    if(AgentGetState() == AGENT_STATE_WAITING_TO_SEND){
        count++;
    }
    AgentSetState(AGENT_STATE_END_SCREEN);
    if(AgentGetState() == AGENT_STATE_END_SCREEN){
        count++;
    }
    if(count == 7){
        printf("AgentSetState() and AgentGetState PASSED!!!\n\n");
    } else{
        printf("AgentSetState() and AgentGetState FAILED!!!\n\n");
    }
    
    printf("Testing AgentInit()...\n");
    AgentInit();
    if(AgentGetState()== AGENT_STATE_START){
        printf("AgentInit() PASSED!!!\n");
    }else{
        printf("AgentInit() FAILED!!!\n\n");
    }
    
    printf("Testing AgentRun()...\n");
    count = 0;
    BB_Event event;
    event.type = BB_EVENT_START_BUTTON;
    event.param0 = 0;
    event.param1 = 0;
    event.param2 = 0;
    AgentRun(event);
    if(AgentGetState()== AGENT_STATE_CHALLENGING){
        printf("AgentRun() PASSED!!!\n");
    }else{
        printf("AgentRun() FAILED!!!\n\n");
    }\

    while(1);
}