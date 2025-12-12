#ifdef DEV_MODE

#include "ai.h"

AiPopulation population = {0};
bool useAiInput = true;
int frameIndex = 0;

void InitPopulation()
{
    population.generation = 0;
    population.currentAgent = 0;

    for (int i = 0; i < POPULATION_SIZE; i++)
    {

        AiAgent *agent = &population.agents[i];

        for (int j = 0; j < AGENT_FRAMES; j++)
        {
            agent->actions[j].thrust = rand() % 2;
            agent->actions[j].turn = rand() % 3 - 1;
        }
    }
}

void RunCurrentAiAgent()
{

    AiAgent *agent = &population.agents[population.currentAgent];

    if (frameIndex >= AGENT_FRAMES)
    {
        FinishAgent(agent);
        return;
    }

    bool thrust = agent->actions[frameIndex].thrust;
    float turn = agent->actions[frameIndex].turn;

    if (thrust) player.activeAnimation = PLAYER_FLY;
    else player.activeAnimation = PLAYER_IDLE;

    UpdatePlayerPhysics(thrust, turn);

    frameIndex++;

    if (playerFinished)
        FinishAgent(agent);
}

void FinishAgent(AiAgent *agent)
{

    agent->time = movementTimer;
    population.currentAgent++;
    frameIndex = 0;

    PrepareGame();
    playerFinished = false;
    movementActivated = true;

    if (useAiInput)
    {
        countdownStarted = true;
        countdownFinished = true;
    }

    StartGame();
}

void DrawAiGUI()
{

    if (useAiInput)
    {
        DrawText(TextFormat("GEN: %d  AGENT: %d  FRAME: %d",
                population.generation,
                population.currentAgent,
                frameIndex),
                GAME_WIDTH/2 - 200, 20, 30, WHITE);
    }
}

#endif