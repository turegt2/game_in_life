#include "Game.h"
#include <fstream>
#include <sstream>
#include <algorithm>

void Game::loadConfig(const std::string& configPath)
{
    std::ifstream file(configPath);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line))
    {
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
            line = line.substr(0, commentPos);
        
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '='))
        {
            std::string value;
            if (std::getline(iss, value))
            {
                key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
                value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
                
                if (key == "width") width = std::stoi(value);
                else if (key == "height") height = std::stoi(value);
                else if (key == "survive")
                {
                    surviveRule.clear();
                    std::stringstream ss(value);
                    int n;
                    while (ss >> n) surviveRule.push_back(n);
                }
                else if (key == "born")
                {
                    bornRule.clear();
                    std::stringstream ss(value);
                    int n;
                    while (ss >> n) bornRule.push_back(n);
                }
            }
        }
    }
}