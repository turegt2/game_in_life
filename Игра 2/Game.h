#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

class Game
{
    int width = 20;
    int height = 20;
    std::vector<int> surviveRule = {2, 3};
    std::vector<int> bornRule = {3};
    
public:
    Game() = default;
    Game(const std::string& configPath) { loadConfig(configPath); }
    
    void loadConfig(const std::string& configPath);
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const std::vector<int>& getSurviveRule() const { return surviveRule; }
    const std::vector<int>& getBornRule() const { return bornRule; }
};

#endif