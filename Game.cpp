#include <sstream>
#include <iomanip>
#include <set>
#include "Game.h"
#include "Simple.h"
#include "Strategic.h"
#include "Food.h"
#include "Advantage.h"

namespace Gaming
{
    //from README file for PA4
    const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;
    const unsigned int Game::MIN_WIDTH = 3;
    const unsigned int Game::MIN_HEIGHT = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;
    PositionRandomizer Game::__posRandomizer = PositionRandomizer();

    //Populate the grid for the game
    //call game calss with populate member function

    void Game::populate()  // populate the grid (used in automatic random initialization of a Game)
    {
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
        //-------------------------------------------------------
        unsigned int numStrategic = __numInitAgents / 2;
        unsigned int numSimple = __numInitAgents - numStrategic;
        unsigned int numAdvantages = __numInitResources / 4;
        unsigned int numFoods = __numInitResources - numAdvantages;


        //The following sections are used to populate the multiple
        //values such as strategic agents, aggressive agents, food,
        //resources, etc.
        //Strategic Agents
        while (numStrategic > 0)
        {
            int i = d(gen);      //passing parameter gen which holds random number engine.

            //if the grid array is equal to null
            if (i != (__width * __height) && __grid[i] == nullptr)
            {
                Position pos(i / __width, i % __width);
                __grid[i] = new Strategic(*this, pos, STARTING_AGENT_ENERGY);
                numStrategic--;
            }
        }


        //populate the Simple Agents
        while (numSimple > 0) {
            int n = d(gen);
            if (__grid[n] == nullptr) {
                Position player(n / __width, n & __width);
                __grid[n] = new Simple(*this, player, STARTING_AGENT_ENERGY);
                numSimple--;
            }
        }


        //Same format as agent factor generator
        //but using resource factor this time.
        while (numFoods > 0)
        {
            int i = d(gen);
            if (i != (__width * __height) && __grid[i] == nullptr)
            {
                Position pos(i / __width, i % __width);
                __grid[i] = new Food(*this, pos, STARTING_RESOURCE_CAPACITY);
                numFoods--;
            }
        }


        //Calculate Advantage
        while (numAdvantages > 0)
        {
            int i = d(gen);
            if (i != (__width * __height) && __grid[i] == nullptr)
            {
                Position pos(i / __width, i % __width);
                __grid[i] = new Advantage(*this, pos, STARTING_RESOURCE_CAPACITY);
                numAdvantages--;
            }
        }
    }


    //Default Ctor
    //Default ctor will set the game to
    //a 3x3 grid and will push a nullptr
    //into each elemted using a for loop.
    Game::Game() : __width(3), __height(3)
    {
        for (unsigned i = 0; i < (__width * __height); ++i)
        {
            __grid.push_back(nullptr);
        }
        __status = NOT_STARTED;
        __verbose = false;
        __round = 0;
    }

    //Overloaded Ctor
    /*This constructor will allow the grid size to be set manually.
     * creates width and height variables that are passed into the
     * original __width and __height from the class. */
    Game::Game(unsigned width, unsigned height, bool manual) : __width(width), __height(height)
    {
        if (width < MIN_WIDTH || height < MIN_HEIGHT)
        {
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);
        }

        __status = NOT_STARTED;
        __verbose = false;
        __round = 0;

        for (unsigned i = 0; i < (__width * __height); ++i)
        {
            __grid.push_back(nullptr);
        }

        if (!manual) { populate();}     //if not manual, call the populate function.
    }


    //Dtor
    //Responsible for destroying the
    //game objects when they are no
    //longer needed.
    Game::~Game()
    {
        for (auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            if (*it != nullptr)
            {
                delete *it;
            }
        }
    }

//-------------------------------------------------------------------------------------------------------

    /*For all get functions, dynamic_cast returns a pointer of the
     * argument type or null if the derived type of the object is not
     * the correct argument type.*/

    //Function to search all pieces on the grid
    //Using provided getNumPieces Function

    // Accessors
    unsigned int Game::getNumPieces() const
    {
        unsigned int numPieces = 0;
        for (auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            if (*it != nullptr)
                numPieces++;
        }

        return numPieces;
    }

    //Repeat the process above but this time to find
    //the number of agents on the grid.
    unsigned int Game::getNumAgents() const
    {
        unsigned int numAgents = 0;

        for (auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            Agent *simple = dynamic_cast<Agent*>(*it);
            if (simple) numAgents++;
        }

        return numAgents;
    }


    //Get number of simple agents on the grid.
    unsigned int Game::getNumSimple() const
    {
        unsigned int numAgents = 0;

        for (auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            Simple *simple = dynamic_cast<Simple*>(*it);
            if (simple) numAgents ++;
        }

        return numAgents;
    }


    //Get number of strategies on the grid
    unsigned int Game::getNumStrategic() const
    {
        unsigned int numAgents = 0;

        for (auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            Strategic *simple = dynamic_cast<Strategic*>(*it);
            if (simple) numAgents ++;
        }

        return numAgents;
    }

    //Get number of resource on the grid
    unsigned int Game::getNumResources() const
    {
        unsigned int numAgents = 0;

        for (auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            Resource *simple = dynamic_cast<Resource*>(*it);
            if (simple) numAgents++;
        }

        return numAgents;
    }

    const Piece *Game::getPiece(unsigned int x, unsigned int y) const
    {
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[y + (x * __width)] == nullptr) throw PositionEmptyEx(x, y);
        return __grid[y + (x * __width)];
    }

    // grid population methods
    void Game::addSimple(const Position &position)
    {
        //Throw exception if out of bounds.
        int index = position.y + (position.x * __width);
        if (position.y >= __width || position.x >= __height) throw OutOfBoundsEx(__width, __height, position.x, position.y);
        if (__grid[index]) throw PositionNonemptyEx(position.x, position.y);

        __grid[index] = new Simple(*this, position, STARTING_AGENT_ENERGY);
    }

    void Game::addSimple(const Position &position, double energy)
    {

        int index = position.y + (position.x * __width);
        if (position.y >= __width || position.x >= __height) throw OutOfBoundsEx(__width, __height, position.x, position.y);
        if (__grid[index]) throw PositionNonemptyEx(position.x, position.y);

        __grid[index] = new Simple(*this, position, energy);
    }

    void Game::addSimple(unsigned x, unsigned y)
    {
        int index = y + (x * __width);
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[index]) throw PositionNonemptyEx(x, y);

        __grid[index] = new Simple(*this, Position(x, y), STARTING_AGENT_ENERGY);
    }

    void Game::addSimple(unsigned y, unsigned x, double energy)
    {
        //exception for out of bounds
        int index = y + (x * __width);
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[index]) throw PositionNonemptyEx(x, y);

        __grid[index] = new Simple(*this, Position(x, y), energy);
    }

    void Game::addStrategic(const Position &position, Strategy *s)
    {
        int index = position.y + (position.x * __width);
        if (position.y >= __width || position.x >= __height) throw OutOfBoundsEx(__width, __height, position.x, position.y);
        if (__grid[index]) throw PositionNonemptyEx(position.x, position.y);

        __grid[index] = new Strategic(*this, position, STARTING_AGENT_ENERGY, s);
    }

    void Game::addStrategic(unsigned x, unsigned y, Strategy *s)
    {
        int index = y + (x * __width);
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[index]) throw PositionNonemptyEx(x, y);

        __grid[index] = new Strategic(*this, Position(x, y), STARTING_AGENT_ENERGY, s);
    }

    void Game::addFood(const Position &position)
    {
        int index = position.y + (position.x * __width);
        if (position.y >= __width || position.x >= __height) throw OutOfBoundsEx(__width, __height, position.x, position.y);
        if (__grid[index]) throw PositionNonemptyEx(position.x, position.y);

        __grid[index] = new Food(*this, position, STARTING_RESOURCE_CAPACITY);
    }

    void Game::addFood(unsigned x, unsigned y)
    {
        int index = y + (x * __width);
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[index]) throw PositionNonemptyEx(x, y);

        __grid[index] = new Food(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
    }

    void Game::addAdvantage(const Position &position)
    {
        int index = position.y + (position.x * __width);
        if (position.y >= __width || position.x >= __height) throw OutOfBoundsEx(__width, __height, position.x, position.y);
        if (__grid[index]) throw PositionNonemptyEx(position.x, position.y);

        __grid[index] = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
    }

    void Game::addAdvantage(unsigned x, unsigned y)
    {
        int index = y + (x * __width);
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[index]) throw PositionNonemptyEx(x, y);

        __grid[index] = new Advantage(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
    }

    const Surroundings Game::getSurroundings(const Position &pos) const
    {

        Surroundings surrounds;     //Declaring array of class Surroundings objects.
        for (int n = 0; n < 9; ++n)     //n is less than 9 since count starts at
                                        //0 and we have only 9 blocks.
        {
            surrounds.array[n] = EMPTY;
        }
        for (int row = -1; row <= 1; ++row)
        {
            for (int column = -1; column <= 1; ++column)
            {
                if (pos.x + row >= 0 && pos.x + row < __height
                    && pos.y + column >= 0 && pos.y + column < __width)
                {
                    // In bounds
                    unsigned int index = pos.y + column + ((pos.x + row) * __width);

                    if (__grid[index])
                        surrounds.array[column + 1 + ((row + 1) * 3)] = __grid[index]->getType();
                }
                else
                {
                    // Out of bounds
                    surrounds.array[column + 1 + ((row + 1) * 3)] = INACCESSIBLE;
                }
            }
        }
        surrounds.array[4] = SELF;

        return surrounds;
    }

    // gameplay methods
    const ActionType Game::reachSurroundings(const Position &from, const Position &to)  // note: STAY by default
    {
        //create a base condition
        //if the from and to locations are equivalent, return STAY
        //because the piece does not move.
        if(from.x == to.x && from.y == to.y)
        {
            return STAY;
        }

        //Pieces move according to compass locations
        //N, S, E, W, NE, NW, SE, and SW.

        if(to.x == from.x -1 && to.y == from.y)
            return N;            //moved up one on grid

        else if(to.x == from.x+1 && to.y == from.y)
            return S;           //moved down one on grid

        else if(to.x == from.x && to.y == from.y+1)
            return E;           //moved right on the grid

        else if(to.x == from.x && to.y == from.y-1)
            return W;           //moved left one on the grid

        else if(to.x == from.x + 1 && to.y == from.y + 1)
            return SE;          //moved down one and right one on the grid

        else if(to.x == from.x-1 && to.y == from.y +1)
            return NE;          //moved up one and right one on the grid

        else if(to.x == from.x -1 && to.y == from.y - 1)
            return NW;           //moved up one and left one

        else if(to.x == from.x +1 && to.y == from.y -1)
            return SW;          //moved down one and left one on the grid
    }//end of reach surroundings

    bool Game::isLegal(const ActionType &ac, const Position &pos) const
    {
        Surroundings surrounds;
        surrounds = getSurroundings(pos);       //surrounds is set equal to getSurroundings
        //and takes a ref Position object.
        bool isLegal = true;        //bool variable initialized to true.
        //can also be false, but expressions will have to
        //match accordingly.

        //Create switch statement for actions
        //pass in Action type reference as parameter to the switch.
        //Since we are dealing with a 3x3 grid with a total of 9 spaces
        //each switch statement case represents a number in the grid that
        //corresponds to each direction.
        //Note that the hightest number is 8 since we are dealing with an
        //array and counting still starts at 0.
        switch (ac){
            case N:
                if (surrounds.array[1] != INACCESSIBLE)
                    isLegal = true;
                break;
            case NE:
                if (surrounds.array[2] != INACCESSIBLE)
                    isLegal = true;
                break;
            case NW:
                if (surrounds.array[0] != INACCESSIBLE)
                    isLegal = true;
                break;
            case E:
                if (surrounds.array[5] != INACCESSIBLE)
                    isLegal = true;
                break;
            case W:
                if (surrounds.array[3] != INACCESSIBLE)
                    isLegal = true;
                break;
            case SE:
                if (surrounds.array[8] != INACCESSIBLE)
                    isLegal = true;
                break;
            case SW:
                if (surrounds.array[6] != INACCESSIBLE)
                    isLegal = true;
                break;
            case S:
                if (surrounds.array[7] != INACCESSIBLE)
                    isLegal = true;
                break;
            case STAY:
                isLegal = true;
                break;
        }
        //each case will be for what direction it moves in the switch statement

        return isLegal;                 //if its true or false
    }

    const Position Game::move(const Position &pos, const ActionType &ac) const  // note: assumes legal, use with isLegal()
    {
        if (isLegal(ac, pos))
        {
            int x, y;
            x = pos.x;
            y = pos.y;
            switch (ac)
            {
                case E: y++;
                    break;
                case NE: y++; x--;
                    break;
                case N: x--;
                    break;
                case NW: y--; x--;
                    break;
                case W: y--;
                    break;
                case SW: y--; x++;
                    break;
                case S: x++;
                    break;
                case SE: x++; y++;
                    break;
                default:
                    break;
            }
            Position neutral((unsigned )x, (unsigned)y);

            return neutral; //never roll in neutral.
        }
        return pos;

    }

    void Game::round()     // play a single round
    {
        std::set<Piece*> pieces;
        for (auto it = __grid.begin(); it != __grid.end(); ++it)
        {
            if (*it)
            {
                pieces.insert(pieces.end(), *it);
                (*it)->setTurned(false);
            }
        }

        //Take turns
        for (auto it = pieces.begin(); it != pieces.end(); ++it)
        {
            if (!(*it)->getTurned())
            {
                (*it)->setTurned(true);
                (*it)->age();
                ActionType ac = (*it)->takeTurn(getSurroundings((*it)->getPosition()));     //each piece takes a turn.

                Position pos0 = (*it)->getPosition();        //passes in a piece position or location
                Position pos1 = move(pos0, ac);

                //if a piece wants to move to a position where there is already another piece,
//              //it will challenge it or consume it's resource.
                if (pos0.x != pos1.x || pos0.y != pos1.y)
                {
                    //if a piece wants to move to a position where there is already another piece,
//                //it will challenge it or consume it's resource.
                    Piece *p = __grid[pos1.y + (pos1.x * __width)];
                    if (p)
                    {
                        (*(*it)) * (*p);        //Statement allows pieces to interact.

                        if ((*it)->getPosition().x != pos0.x || (*it)->getPosition().y != pos0.y)
                            //if item isnt' consumed, set it to a new position.
                        {
                            // piece moved
                            __grid[pos1.y + (pos1.x * __width)] = (*it);
                            __grid[pos0.y + (pos0.x * __width)] = p;
                        }
                    } else
                    {
                        // empty move
                        //when a resource is consumed, set the
//                      //location to nullptr since there will
//                      //no longer be anything there.
                        (*it)->setPosition(pos1);
                        __grid[pos1.y + (pos1.x * __width)] = (*it);        //changes the grid and sets it equal to the
//                                                                          //*it reference.
                        __grid[pos0.y + (pos0.x * __width)] = nullptr;      //sets the grid equal to nullptr.
                    }
                }
            }
        }

        // Update positions and delete
        // Delete invalid first
        for (unsigned int i = 0; i < __grid.size(); ++i)
        {
            if (__grid[i] && !(__grid[i]->isViable()))
            {
                delete __grid[i];
                __grid[i] = nullptr;
            }
        }

        // Check game over
        if (getNumResources() <= 0)
        {
            __status = Status::OVER;
        }
        __round++;
    }

    void Game::play(bool verbose)   // play game until over
    {
        __verbose = verbose;
        __status = PLAYING;     //returns the status that the game is playing

        std::cout << *this;     //Prints out what the game is doing at the current time.

        //While the game is not OVER, and is in progress,
        //execute the following statements.
        while (__status != OVER)
        {
            round();
            if (verbose) std::cout << *this;
        }
        if (!verbose) std::cout << *this;
        //Prints out the location of pieces on the board
        //when the game is over.
    }

    std::ostream &operator<<(std::ostream &os, const Game &game)
    {
        os << "Round " << game.__round << std::endl;
        int column = 0;
        for (auto it = game.__grid.begin(); it != game.__grid.end(); ++it)
        {
            if (*it == nullptr)
            {
                os << "[" << std::setw(6) << "]";
            } else
            {
                //os ;
                std::stringstream ss;
                ss << "[" << **it;
                std::string str;
                std::getline(ss, str);
                //os << str << std::setw(3) << "]";
                os << str << "]";
                //os << std::setw(4) << "[" << **it;
                //os << "]";
                //os ;
            }
            if (++column == game.__width)
            {
                column = 0;
                os << std::endl;
            }
        }
        os << "Status: ";
        switch (game.getStatus())
        {
            case Game::Status::NOT_STARTED:
                std::cout << "Not Started..." << std::endl; break;
            case Game::Status::PLAYING:
                std::cout << "Playing..." << std::endl; break;
            default:
                std::cout << "Over!" << std::endl; break;
        }
        return os;
    }
}   //END OF NAMESPACEGAMING SCOPE