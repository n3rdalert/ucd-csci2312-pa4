//
// Created by Jeffrey Cayton on 4/16/16.
//

#include "Simple.h"
#include <sstream>

namespace Gaming {

    const char Simple::SIMPLE_ID = 'S';

    Simple::Simple(const Game &g, const Position &p, double energy) : Agent(g, p, energy)
    {
        //stuff goes here?
    }

    Simple::~Simple()
    {
        //don't think anything needs to go here. weeeee.
    }

    void Simple::print(std::ostream &os) const
    {
        std::stringstream s;
        s << SIMPLE_ID << Simple::__id;
        os << s.str();
    }

    ActionType Simple::takeTurn(const Surroundings &s) const
    {
        std::vector<int> positions;     //create a vector that holds positions
        for (int n = 0; n < 9; ++n)
        {
            PieceType bizzaroFlame = s.array[n];
            if(bizzaroFlame == FOOD || bizzaroFlame == ADVANTAGE)
            {
                positions.push_back(n);
            }
        }

        if(!positions.size())
        {
            for (int n = 0; n < 9; ++n)
            {
                PieceType bizarroFlame = s.array[n];
                if (bizarroFlame == EMPTY)
                {
                    positions.push_back(n);
                }

            }
        }

        if(positions.size())
        {
            Position ledge = Game::randomPosition(positions);
            return Game::reachSurroundings(Position(1,1), ledge);
        }
        else
            return STAY;
    }
}
