//
// Created by Jeffrey Cayton on 4/16/16.
//

#include <sstream>
#include "Food.h"

namespace Gaming {

    const char Food::FOOD_ID = 'F';

    Food::Food(const Game &g, const Position &p, double capacity) : Resource(g, p, capacity)
    {
        __capacity = capacity;
    }

    Food::~Food() { }

    void Food::print(std::ostream &os) const
    {
        //create string stream object
        std::stringstream words;

        words << FOOD_ID << Food::__id;
        os << words.str();
    }
}