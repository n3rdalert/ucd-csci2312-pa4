//
// Created by Jeffrey Cayton on 4/16/16.
//


#include "Advantage.h"

namespace Gaming {

    const char Advantage::ADVANTAGE_ID = 'D';
    const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;

    Advantage::Advantage(const Game &g, const Position &p, double capacity) : Resource(g, p, capacity) {

    }
}
