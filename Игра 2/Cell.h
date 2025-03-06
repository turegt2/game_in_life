#ifndef CELL_H
#define CELL_H

class Cell
{
    bool alive;
public:
    Cell() : alive(false) {}
    bool isAlive() const { return alive; }
    void setAlive(bool state) { alive = state; }
};

#endif