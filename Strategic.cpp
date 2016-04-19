//
// Created by Jeffrey Cayton on 4/16/16.
//

#include "Strategic.h"
#include <sstream>

namespace Gaming
{
    const char Strategic::STRATEGIC_ID = 'T';

    Strategic::Strategic(const Game &g, const Position &p, double energy, Strategy *s):Agent(g, p, __energy)
    {
        __strategy = s;         //loads the default strategy.
    }


    Strategic::~Strategic()
    {
        delete __strategy;
    }

    void Strategic::print(std::ostream &os) const
    {
        std::stringstream s;
        s << STRATEGIC_ID << Strategic::__id;
        os << s.str();
    }

    ActionType Strategic::takeTurn(const Surroundings &s) const
    {
        return S;
    }
}