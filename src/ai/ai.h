#ifndef AI_H
#define AI_H

#ifdef DEV_MODE

#include "raylib.h"
#include "globals.h"
#include "player.h"
#include "gui.h"
#include <stdlib.h>

#define POPULATION_SIZE 50
#define AGENT_FRAMES 2000

typedef struct
{
    bool thrust;
    float turn;
} AiInput;

typedef struct
{
    AiInput actions[AGENT_FRAMES];

    float time;
    bool finished;
} AiAgent;

typedef struct {
    AiAgent agents[POPULATION_SIZE];
    int generation;
    int currentAgent;
} AiPopulation;

extern AiPopulation population;
extern bool useAiInput;

void InitPopulation();
void RunCurrentAiAgent();
void FinishAgent(AiAgent *agent);
void DrawAiGUI();

#endif
#endif