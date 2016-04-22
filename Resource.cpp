//
// Created by Jeffrey Cayton on 4/16/16.
//

#include "Resource.h"
#include "Agent.h"

namespace Gaming {

    const double Resource::RESOURCE_SPOIL_FACTOR = 1.2;

    //Default Ctor
    Resource::Resource(const Game &g, const Position &p, double capacity):Piece(g,p) {
        __capacity = capacity;
    }

    //Dtor
    Resource::~Resource() { }

    double Resource::consume()
    {
        this->finish();
        return __capacity;
    }


    ActionType Resource::takeTurn(const Surroundings &s) const
    {
        return STAY;        //returns STAY position since there are no resources around.
    }

    Piece &Resource::operator*(Piece &other)
    {
        return other.interact(this);
    }

    void Resource::age()
    {
        __capacity /= RESOURCE_SPOIL_FACTOR;
        if(__capacity < 0.01)
            __capacity =0;
    }

    Piece &Resource::interact(Agent *agent)
    {
        agent->addEnergy(consume());
        return *this;
    }

    Piece &Resource::interact(Resource *resource) {
        return *this;
    }
}