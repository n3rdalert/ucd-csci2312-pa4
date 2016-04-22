//
// Created by Jeffrey Cayton on 4/16/16.
//

#include "Piece.h"

namespace Gaming {

    unsigned int Piece::__idGen = 1000;  //set idGen to 1000

    //Use Ctor from the Piece class declaration
    Piece::Piece(const Game &g, const Position &p) : __game(g), __position(p)
    //passes value of g into game, and passes p into parameter.
    {
        __finished = false; //Initialized to false since this is start of game.
        __turned = false;
        __id = __idGen++;     //set id to value of idGen.
    }

    //Dtor for Piece
    Piece::~Piece() {
        /*Stuff may need to go in here*/
    }


    //Friend function for piece class has access to private class
    //members. This will allow the output stream to be returned.
    //Parameters will be an ostream object reference and a piece
    //object reference.
    std::ostream &operator<<(std::ostream &os, const Piece &piece)
    {
        piece.print(os);
        return os;
    }

} //end of namespace gaming.
