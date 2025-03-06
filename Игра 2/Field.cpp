#include "Field.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <algorithm>

Field::Field(Game& game) : game(game)
{
    std::srand(std::time(nullptr));
    grid.resize(game.getHeight(), std::vector<Cell>(game.getWidth()));
    
    for (auto& row : grid)
        for (auto& cell : row)
            cell.setAlive(std::rand() % 2);
}

int Field::countNeighbors(int x, int y) const
{
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0) continue;
            
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < game.getWidth() && ny >= 0 && ny < game.getHeight() && grid[ny][nx].isAlive())
            {
                count++;
            }
        }
    }
    return count;
}

void Field::nextStep()
{
    auto newGrid = grid;
    for (int y = 0; y < game.getHeight(); ++y)
    {
        for (int x = 0; x < game.getWidth(); ++x)
        {
            int neighbors = countNeighbors(x, y);
            bool alive = grid[y][x].isAlive();
            bool newState = false;
            
            if (alive)
            {
                auto& rule = game.getSurviveRule();
                if (std::find(rule.begin(), rule.end(), neighbors) != rule.end())
                    newState = true;
            } else
            {
                auto& rule = game.getBornRule();
                if (std::find(rule.begin(), rule.end(), neighbors) != rule.end())
                    newState = true;
            }
            
            newGrid[y][x].setAlive(newState);
        }
    }
    grid = std::move(newGrid);
}

void Field::display() const
{
    for (const auto& row : grid)
    {
        for (const auto& cell : row)
            std::cout << (cell.isAlive() ? "#" : " ") << " ";
        std::cout << "\n";
    }
}