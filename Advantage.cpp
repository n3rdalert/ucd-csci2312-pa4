//
// Created by Jeffrey Cayton on 4/16/16.
//


#include "Advantage.h"
#include <sstream>

namespace Gaming {

    const char Advantage::ADVANTAGE_ID = 'D';
    const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;

    Advantage::Advantage(const Game &g, const Position &p, double capacity) : Resource(g, p, capacity) { }

    Advantage::~Advantage() { }

    void Advantage::print(std::ostream &os) const
    {
        //use the function from class
        std::stringstream s;
        s << ADVANTAGE_ID << Advantage::__id;
        os << s.str();          //inserts string stream object into ostream object
    }

    double Advantage::getCapacity() const
    {
        return Advantage::__capacity;
    }

    double Advantage::consume()
    {
        return __capacity;          //returns capacity since it will get consumed
    }
}
