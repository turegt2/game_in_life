#include "Game.h"
#include "Field.h"
#include <iostream>
#include <limits>

int main()
{
    Game game("config.txt");
    Field field(game);
    
    while (true)
    {
        std::system("cls || clear");
        field.display();
        field.nextStep();
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    return 0;
}