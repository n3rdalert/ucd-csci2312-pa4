//
// Created by Jeffrey Cayton on 4/14/16.
//
#include "Game.h"
#include "Advantage.h"
#include "Food.h"
#include "Strategic.h"
#include "Simple.h"

namespace Gaming {

    //from README file for PA4
    const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;
    const unsigned int Game::MIN_WIDTH = 3;
    const unsigned int Game::MIN_HEIGHT = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;

    //Populate the grid for the game
    //call game calss with populate member function

    void Game::populate() {
        //pseudo-random number generator used
        //for this particular program. Nothing fancy.
        //-------------------------------------------
        std::default_random_engine gen;                                   //gen holds random number gen engine.

        //Parameters of uniform distribution are 0, the center,
        //and the next position in the grid which is represented
        //as width * height.
        //----------------------------------------------------------
        std::uniform_int_distribution<int> d(0, __width * __height);      //uniform_int_dist is a vector of ints

        __numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
        __numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;

        //Declaration of numAdvantage needs to be up here in order
        //for it to be in scope of calculate food section.
        //---------------------------------------------------------
        unsigned int numAdvantage = __numInitResources / 4;

        //Calculate number of Strategic Agents
        unsigned int numStrategic = __numInitAgents / 2;



        //The following sections are used to populate the multiple
        //values such as strategic agents, aggressive agents, food,
        //resources, etc.
        //Strategic Agents
        while (numStrategic > 0) {
            int n = d(gen);      //passing parameter gen which holds random number engine.

            //if the grid array is equal to null
            if (__grid[n] == nullptr) {
                Position player(n / __width, n % __width);
                __grid[n] = new Strategic(*this, player, STARTING_AGENT_ENERGY);
                numStrategic--;
            }
        }


        unsigned int numSimple = __numInitAgents - numStrategic;
        //populate the Simple Agents
        while (numSimple > 0) {
            int n = d(gen);
            if (__grid[n] == nullptr) {
                Position player(n / __width, n & __width);
                __grid[n] = new Simple(*this, player, STARTING_AGENT_ENERGY);
                numSimple--;
            }
        }



        //Calculate Food
        unsigned int numFood = __numInitResources - numAdvantage;
        //Same format as agent factor generator
        //but using resource factor this time.
        while (numFood > 0) {
            int n = d(gen);
            if (__grid[n] == nullptr) {
                Position player(n / __width, n % __width);
                __grid[n] = new Food(*this, player, STARTING_RESOURCE_CAPACITY);
                numFood--;
            }
        }



        //Calculate Advantage

        while (numAdvantage > 0) {
            int n = d(gen);
            if (__grid[n] == nullptr) {
                Position player(n / __width, n % __width);
                __grid[n] = new Advantage(*this, player, Game::STARTING_RESOURCE_CAPACITY);
                numAdvantage--;
            }

        }

    }   //end of void Game::populate scope



    //Default Ctor
    //Default ctor will set the game to
    //a 3x3 grid and will push a nullptr
    //into each elemted using a for loop.
    Game::Game() : __width(3), __height(3) {
        for (int n = 0; n < (__width * __height); ++n) {
            __grid.push_back(nullptr);
            //since the grid is a vector, we can
            //use the push back function from the
            //vector STL.
        }
    }

    //Overloaded Ctor
    /*This constructor will allow the grid size to be set manually.
     * creates width and height variables that are passed into the
     * original __width and __height from the class. */
    Game::Game(unsigned int width, unsigned int height, bool manual) : __width(width), __height(height) {
        for (int n = 0; n < __width; ++n) {
            __grid.push_back(nullptr);
        }

        if (!manual) { populate(); }      //if not manual, call populate function.
    }


    //Dtor
    //Responsible for destroying the
    //game objects when they are no
    //longer needed.
    Game::~Game() { }
    //-------------------------------------------------------------------------------------------------------

    /*For all get functions, dynamic_cast returns a pointer of the
     * argument type or null if the derived type of the object is not
     * the correct argument type.*/

    //Function to search all pieces on the grid
    //Using provided getNumPieces Function
    unsigned int Game::getNumPieces() const {
        unsigned int numPieces = 0; //keeps track of total pieces

        for (auto item = __grid.begin(); item != __grid.end(); ++item) {
            if (*item != nullptr) {
                numPieces++;
            }
        }

        return numPieces;
    }


    //Repeat the process above but this time to find
    //the number of agents on the grid.
    unsigned int Game::getNumAgents() const {
        unsigned int numAgents = 0;
        for (auto item = __grid.begin(); item != __grid.end(); ++item) {
            Agent *agent = dynamic_cast<Agent *> (*item);
            if (agent) numAgents++;
        }

        return numAgents;
    }


    //Get number of simple agents on the grid.
    unsigned int Game::getNumSimple() const {
        unsigned int numSimple = 0;

        for (auto item = __grid.begin(); item != __grid.end(); ++item) {
            Simple *simple = dynamic_cast<Simple *> (*item);
            if (simple) numSimple++;
        }

        return numSimple;
    }


    //Get number of strategies on the grid
    unsigned int Game::getNumStrategic() const
    {
        unsigned int numStrategic = __numInitAgents / 2;
        for (auto item = __grid.begin(); item != __grid.end(); ++item) {
            Strategic *strategic = dynamic_cast<Strategic *> (*item);
            if (strategic) numStrategic++;
        }

        return numStrategic;
    }


    //Get number of resource on the grid
    unsigned int Game::getNumResources() const
    {
        unsigned int numResource = 0;
        for (auto item = __grid.begin(); item != __grid.end(); ++item) {
            Resource *resource = dynamic_cast<Resource *> (*item);
            if (resource) numResource++;
        }

        return numResource;
    }


    const Piece *Game::getPiece(unsigned int x, unsigned int y) const
    {
        int location = y + (x * __width);
        return __grid[location];
    }


    void Game::addSimple(const Position &position) {
        //Throw exception if out of bounds.
        if ((position.x > 0 && position.x < getWidth()) || (position.y > 0 && position.y < getHeight())) {
            throw OutOfBoundsEx(getWidth(), getHeight(), position.x, position.y);
        }
        int location = position.y + (position.x * __width);
        __grid[location] = new Simple(*this, position, STARTING_AGENT_ENERGY);

    }


    void Game::addSimple(const Position &position, double energy) {
        if ((position.x < 0 && position.x > getWidth()) || (position.y < 0 && position.y > getHeight())) {
            throw OutOfBoundsEx(getWidth(), getHeight(), position.x, position.y);
        }
        int location = position.y + (position.x * __width);
        __grid[location] = new Simple(*this, position, energy);
    }


    void Game::addSimple(unsigned x, unsigned y) {
        //exception for out of bounds
        if ((x < 0 && x > getWidth()) || (y < 0 && y > getHeight())) {
            throw OutOfBoundsEx(getWidth(), getHeight(), x, y);
        }
        Position pos(x, y);
        int location = y + (x * __width);
        __grid[location] = new Simple(*this, pos, STARTING_AGENT_ENERGY);
    }


    void Game::addSimple(unsigned x, unsigned y, double energy) {
        //exception for out of bounds
        if ((x < 0 && x > getWidth()) || (y < 0 && y > getHeight())) {
            throw OutOfBoundsEx(getWidth(), getHeight(), x, y);
        }
        Position pos(x, y);
        int location = y + (x * __width);
        __grid[location] = new Simple(*this, pos, energy);
    }


    void Game::addStrategic(const Position &position, Strategy *s) {
        int location = position.y + (position.x * __width);
        __grid[location] = new Strategic(*this, position, STARTING_AGENT_ENERGY, s);
    }


    void Game::addStrategic(unsigned x, unsigned y, Strategy *s) {
        Position pos(x, y);
        int location = y + (x * __width);
        __grid[location] = new Strategic(*this, pos, STARTING_AGENT_ENERGY, s);
    }


    void Game::addFood(const Position &position) {
        int location = position.y + (position.x * __width);
        __grid[location] = new Food(*this, position, STARTING_RESOURCE_CAPACITY);
    }


    void Game::addFood(unsigned x, unsigned y) {
        Position pos(x, y);
        int location = y + (x * __width);
        __grid[location] = new Food(*this, pos, STARTING_RESOURCE_CAPACITY);
    }


    void Game::addAdvantage(const Position &position) {
        int location = position.y + (position.x * __width);
        __grid[location] = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
    }


    void Game::addAdvantage(unsigned x, unsigned y) {
        Position pos(x, y);
        int location = y + (x * __width);
        __grid[location] = new Advantage(*this, pos, STARTING_RESOURCE_CAPACITY);
    }


    const Surroundings Game::getSurroundings(const Position &position) const {

        Surroundings surround;              //array of class Surroundings


        for (int n = 0; n < 9; n++) {     //n is less than nine since there's only 8 elements
            if (n == 4)                   //Our position in the grid is equal to block 4.
            {
                surround.array[n] = SELF;
            }

            else
                surround.array[n] = EMPTY;  //sets all elements to enum Empty
        }


        Position search;                    //searches for position
        PieceType p;                        //holds piece that will be found
        search.x = position.x - 1;          //x and y are passed into function as positions
        search.y = position.y - 1;

        //For loops below will traverse the columns and rows.
        //columns are "m", rows are "j"
        for (int m = 0; m < 3; m++)
        {
            for (int j = 0; j < 3; j++)
            {
                //base conditions where the position is outside the grid
                if ((search.x < 0 || search.x > __width) || (search.y < 0 || search.y > __height))
                {
                    surround.array[m + (j * __width)] = INACCESSIBLE;   //will set that position as inaccessible
                }

                else
                {
                    if (__grid[m + (j * __width)] != nullptr)
                    {
                        //base condition where it checks if that part of the grid has a object in it
                        p = __grid[m + (j *
                                        __width)]->getType();   //will get the type of piece or object that is at that part of the grid and will set that equal to our piece type variable p
                        surround.array[m + (j * __width)] = p;
                    }
                    else
                        surround.array[m + (j *
                                            __width)] = EMPTY;  //there's nothing in that index so we set that equal to empty
                }
            }
            search.x = position.x - 1;      //used to reset x to what it should be
            search.y += 1;                  //and the y
        }

        //return surround;                    //will return the Surroundings

        return Gaming::Surroundings();      //Previous return statement is unreachable.
                                            //unsure if this return statement is correct one.
    }

    const ActionType Game::reachSurroundings(const Position &from, const Position &to)
    {
        //base condition
        if (from.x == to.x && from.y == to.y == from.x)
        {
            return STAY;       //no adjacent empty positions
                                //Stays in place.
        }

        //WHERE THE PIECE MOVES (N,S,E,W,NE,NW,SE,SW)//////////////////
        //todo check which one works for N
        //if(to.x == from.x && to.y == from.y == to.y -1)
        //return N
        if(to.x == from.x && to.y == from.y -1)
            return N;            //moved up one on grid
        else if(to.x == from.x && to.y == from.y +1)
            return S;           //moved down one on grid
        else if(to.x == from.x + 1 && to.y == from.y)
            return E;           //moved right on the grid
        else if(to.x == from.x + 1 && to.y == from.y + 1)
            return SE;          //moved down one and right one on the grid
        else if(to.x == from.y +1 && to.y == from.y - 1)
            return NE;          //moved up one and right one on the grid
        else if(to.x == from.x -1 && to.y == from.y)
            return W;           //moved left one on the grid
        else if(to.x == from.x -1 && to.y == from.y - 1)
            return NW;           //moved up one and left one
        else if(to.x == from.x -1 && to.y == from.y -1)
            return SW;          //moved down one and left one on the grid
        ///////////END OF WHERE THE PiECE MOVES ON THE GRID////////////////
    }


    bool Game::isLegal(const ActionType &ac, const Position &pos) const {


        return false;
    }
}//End of namespace Gaming scope.
