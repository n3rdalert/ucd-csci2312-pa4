//
// Created by Jeffrey Cayton on 4/16/16.
//

#include "Game.h"
#include "AggressiveAgentStrategy.h"
#include <ctime>

namespace  Gaming
{

    const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;

    //Default Ctor
    AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy)
    {
        __agentEnergy = agentEnergy;
    }

    //Dtor
    AggressiveAgentStrategy::~AggressiveAgentStrategy() { }


    //strategy for agrressive agent
    ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const
    {

        std::vector<int> moves;
        PositionRandomizer random;      //random generator
        Position p, mid(1,1);
        int location;
        std::default_random_engine gen(time(0));
        bool surround[4] = { false, false, false, false };
        for (int n = 0; n < 8; ++n)
        {
            //If statement checks if there are any agents nearby
            if ((s.array[n] == SIMPLE || s.array[n] == STRATEGIC) && __agentEnergy >= DEFAULT_AGGRESSION_THRESHOLD)
                surround[0] = true;

                //Check for any nearby advantage on the grid.
            else if (s.array[n] == ADVANTAGE)
                surround[1] = true;
                //Check for nearby food on the grid.
            else if (s.array[n] == FOOD)
                surround[2] = true;
                //if nothing else is found, leave empty.
            else if (s.array[n] == EMPTY)
                surround[3] = true;
        }


        if (surround[0])
        {
            for (int n = 0; n < 8; ++n)
            {
                if (s.array[n] == SIMPLE || s.array[n] == STRATEGIC)
                    moves.push_back(n);
            }
            location = moves[gen() % moves.size()];
        }


        else if (surround[1])
        {
            //If no agents were found, moves to advantage.
            for (int n = 0; n < 8; ++n)
            {
                if (s.array[n] == ADVANTAGE)
                    moves.push_back(n);
            }
            location = moves[gen() % moves.size()];
        }

        else if (surround[2])
        {
            //If point in array is empty, moves to food.
            for (int n = 0; n < 8; ++n)
            {
                if (s.array[n] == FOOD)
                    moves.push_back(n);
            }

            location = moves[gen() % moves.size()];
        }
        else if (surround[3])
        {
            for (int n = 0; n < 8; ++n)
            {
                if (s.array[n] == EMPTY)
                    moves.push_back(n);
            }
            location = moves[gen() % moves.size()];
        }


        else//if all the elements = false then noconditions are met for the agent to move
            return STAY;

        switch(location){       //each case will be for each part of the surroundings in the array
            case 0:
                p.x = mid.x -1;
                p.y = mid.y -1;
                break;
            case 1:
                p.x = mid.x-1;
                p.y = mid.y;
                break;
            case 2:
                p.x = mid.x-1;
                p.y = mid.y+1;
                break;
            case 3:
                p.x = mid.x;
                p.y = mid.y-1;
                break;
            case 4:
                p.x = mid.x;
                p.y = mid.y;
                break;
            case 5:
                p.x = mid.x;
                p.y = mid.y+1;
                break;
            case 6:
                p.x = mid.x+1;
                p.y = mid.y-1;
                break;
            case 7:
                p.x = mid.x+1;
                p.y = mid.y;
                break;
            case 8:
                p.x = mid.x+1;
                p.y = mid.y+1;
                break;
        }
        return Game::reachSurroundings(mid, p);
    }
}
