#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <future>

using namespace std;

class Cell
{
private:
    bool alive;
public:
    Cell() : alive(false) {}
    bool isAlive() const { return alive; }
    void setAlive(bool state) { alive = state; }
    void toggle() { alive = !alive; }
};

class Game
{
private:
    int width = 20;
    int height = 20;
    int survive_min = 2;
    int survive_max = 3;
    int birth_min = 3;
    int birth_max = 3;
public:
    void loadConfig(const string& filename)
    {
        ifstream file(filename);
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            string key;
            if (iss >> key)
            {
                if (key == "width") { int val; if (iss >> val) width = val; }
                else if (key == "height") { int val; if (iss >> val) height = val; }
                else if (key == "survive_min") { int val; if (iss >> val) survive_min = val; }
                else if (key == "survive_max") { int val; if (iss >> val) survive_max = val; }
                else if (key == "birth_min") { int val; if (iss >> val) birth_min = val; }
                else if (key == "birth_max") { int val; if (iss >> val) birth_max = val; }
            }
        }
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getSurviveMin() const { return survive_min; }
    int getSurviveMax() const { return survive_max; }
    int getBirthMin() const { return birth_min; }
    int getBirthMax() const { return birth_max; }
};

class Field
{
private:
    int width, height;
    vector<vector<Cell>> grid;
    Game& game;
    
    int countNeighbors(int x, int y)
    {
        int count = 0;
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < width && ny >= 0 && ny < height)
                {
                    if (grid[ny][nx].isAlive()) count++;
                }
            }
        }
        return count;
    }

public:
    Field(Game& g) : game(g)
    {
        width = game.getWidth();
        height = game.getHeight();
        grid.resize(height, vector<Cell>(width));
    }

    void clear()
    {
        for (auto& row : grid)
            for (auto& cell : row)
                cell.setAlive(false);
    }

    void toggleCell(int x, int y)
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
            grid[y][x].toggle();
    }

    void nextStep()
    {
        vector<vector<bool>> newStates(height, vector<bool>(width, false));
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int neighbors = countNeighbors(x, y);
                bool alive = grid[y][x].isAlive();
                
                if (alive)
                {
                    if (neighbors >= game.getSurviveMin() && neighbors <= game.getSurviveMax())
                        newStates[y][x] = true;
                } else
                {
                    if (neighbors >= game.getBirthMin() && neighbors <= game.getBirthMax())
                        newStates[y][x] = true;
                }
            }
        }

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
                grid[y][x].setAlive(newStates[y][x]);
    }

    void display()
    {
        cout << "\033[H\033[J";
        for (const auto& row : grid)
        {
            for (const auto& cell : row)
            {
                cout << (cell.isAlive() ? '#' : '.');
            }
            cout << endl;
        }
    }
};

int main()
{
    Game game;
    game.loadConfig("world.txt");
    Field field(game);
    bool simulationRunning = false;
    
    cout << "Game of Life (Ctrl+C to exit)\nCommands: help, clear, start, stop, x y\n";


    string input;
    while (true) {
        if (!simulationRunning) cout << "> ";
        getline(cin, input);
        
        if (input == "help")
        {
            cout << "Commands:\n"
                 << "x y    - Toggle cell\n"
                 << "clear  - Clear field\n"
                 << "start  - Start simulation\n"
                 << "stop   - Stop simulation\n"
                 << "help   - Show help\n";
        } else if (input == "clear")
        {
            field.clear();
            field.display();
        } else if (input == "start")
        {
            simulationRunning = true;
            field.display();
            
            future<string> inputFuture;
            while (simulationRunning)
            {
                auto stepStart = chrono::steady_clock::now();
                
                field.nextStep();
                field.display();

                if (!inputFuture.valid()) {
                    inputFuture = async(launch::async, []()
                    {
                        string cmd;
                        getline(cin, cmd);
                        return cmd;
                    });
                }

                if (inputFuture.wait_for(chrono::milliseconds(500)) == future_status::ready)
                {
                    string command = inputFuture.get();
                    if (command == "stop")
                    {
                        simulationRunning = false;
                        cout << "Simulation stopped.\n";
                    } else if (!command.empty())
                    {
                        cout << "Unknown command during simulation. Use 'stop' to pause.\n";
                    }
                    inputFuture = {};
                }

                auto stepEnd = chrono::steady_clock::now();
                auto elapsed = chrono::duration_cast<chrono::milliseconds>(stepEnd - stepStart);
                if (elapsed < chrono::milliseconds(500)) {
                    this_thread::sleep_for(chrono::milliseconds(500) - elapsed);
                }
            }
        } else if (input == "stop")
        {
            if (!simulationRunning)
            {
                cout << "Simulation is not running.\n";
            }
        } else {
            istringstream iss(input);
            int x, y;
            if (iss >> x >> y) {
                field.toggleCell(x, y);
                field.display();
            } else {
                cout << "Invalid command!\n";
            }
        }
    }

    return 0;
}