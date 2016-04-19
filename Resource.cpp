//
// Created by Jeffrey Cayton on 4/16/16.
//

#include "Resource.h"
#include "Agent.h"

namespace Gaming {

    const double Resource::RESOURCE_SPOIL_FACTOR = 1.2;

    Resource::Resource(const Game &g, const Position &p, double capacity) : Piece(g, p), __capacity(capacity) {

    }
}