//
// Created by Jeffrey Cayton on 4/16/16.
//

#include "DefaultAgentStrategy.h"
#include "Game.h"

namespace Gaming {
    /*stuff still needs to go here!*/

    //Default Ctor
    DefaultAgentStrategy::DefaultAgentStrategy() { }

    //Dtor
    DefaultAgentStrategy::~DefaultAgentStrategy() { }

    ActionType DefaultAgentStrategy::operator()(const Surroundings &s) const {
        bool surround[4] ={false,false,false,false};        //all locations return false
        PositionRandomizer random;
        Position p, mid(1,1);
        std::vector<int> moves;
        int location;   //holds location of a piece.

        //For loop checks the grid for any present pieces.
        for(int n = 0; n < 8; n++)
        {
            if(s.array[n] == ADVANTAGE)
            {
                surround[0] = true;     //checks for advantage
            }
            else if(s.array[n] == FOOD)
            {
                surround[1] = true;     //checks for food
            }
            else if(s.array[n] == EMPTY)
            {
                surround[2] = true;     //checks for empty space
            }
            else if(s.array[n] == SIMPLE)
            {
                surround[3] = true;     //checks for the presence of a simple piece.
            }
        }


        std::default_random_engine gen(time(0));
        if (surround[0])
        {
            //advatage is prioity
            for (int n = 0; n < 8; ++n) {
                if (s.array[n] == ADVANTAGE)
                    moves.push_back(n);
            }
            location = moves[gen() % moves.size()];
        }
            //if there is not advantage it will check for food
        else if (surround[1])
        {
            for (int n = 0; n < 8; ++n)
            {
                if (s.array[n] == FOOD)
                    moves.push_back(n);
            }
            location = moves[gen() % moves.size()];
        }

            //If there's no food, check for an available
            //empty space to move to.
        else if (surround[2])
        {
            for (int n = 0; n < 8; ++n)
            {
                if (s.array[n] == EMPTY)
                    moves.push_back(n);
            }
            location = moves[gen() % moves.size()];
        }

    //If there is no empty space present, it will
            // have the simple agent attack.
        else if (surround[3]) {
            for (int n = 0; n < 8; ++n)
            {
                if (s.array[n] == SIMPLE)
                    moves.push_back(n);
            }
            location = moves[gen() % moves.size()];
        }
        else
            return STAY;
        //if all surrounding return a false value
        //piece will stay since it is unable to move.


        //Switch statement for surroundings in the array.
        //each statement represents each section of the surrounding
        //grid.
        switch(location){
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