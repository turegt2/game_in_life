#ifndef FIELD_H
#define FIELD_H

#include "Game.h"
#include "Cell.h"
#include <vector>

class Field
{
    Game& game;
    std::vector<std::vector<Cell>> grid;
    
    int countNeighbors(int x, int y) const;
    
public:
    Field(Game& game);
    void nextStep();
    void display() const;
};

#endif