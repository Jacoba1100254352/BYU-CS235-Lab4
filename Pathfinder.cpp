#include "Pathfinder.h"

Pathfinder::Pathfinder() {
    for (auto & row : maze_grid)
        for (auto & col : row)
            for (int & height : col)
                height = 1;
}

Pathfinder::~Pathfinder()= default;

string Pathfinder::toString() const
{
    stringstream ss;
    for(int row = 0; row < X; row++) {
        for (int col = 0; col < Y; col++) {
            for (int height = 0; height < Z; height++)
                height != Z - 1 ? ss << maze_grid[row][col][height] << " " : ss << maze_grid[row][col][height];
            ss << endl;
        }
        if(row != Z - 1)
            ss << endl;
    }

    return ss.str();

}

void Pathfinder::createRandomMaze() {
    for(int row = 0; row < X; row++)
        for(int col = 0; col < Y; col++)
            for(int height = 0; height < Z; height++)
                if( (row == 0 && col == 0 && height == 0) || (row == X - 1 && col == Y - 1 && height == Z - 1) )
                    maze_grid[row][col][height] = 1;
                else {
                    ///   Use random library to be epic and randomize our maze   ///
                    std::random_device rd;
                    std::mt19937 mt(rd());
                    std::uniform_real_distribution<> dist(1, 10000);
                    maze_grid[row][col][height] = static_cast<int>(dist(mt)) % 2;
                }
}

bool Pathfinder::importMaze(string file_name) {

    cout << "importMaze from "<< file_name << endl;

    ///   Variables   ///
    string line;
    int numChar = 0;

    ifstream fileCheck(file_name.c_str());
    ifstream file(file_name.c_str());

    ///   Body   ///
    if (fileCheck.is_open() && file.is_open()) {

        // Check for valid length
        while( getline ( fileCheck, line ) )
            for (char i : line)
                if(isdigit(i))
                    numChar++;
                else if(isalpha(i))
                    return false;

        if(numChar > X*Y*Z || numChar < X*Y*Z)
            return false;

        // Read in location and values
        for(int row = 0; row < X; row++)
            for(int col = 0; col < Y; col++) {
                getline(file, line);
                stringstream ss(line);
                if(!line.empty())
                    for(int height = 0; height < Z; height++) {
                        int value;
                        ss >> value;
                        maze_grid[row][col][height] = value;
                        original_maze_grid[row][col][height] = value;
                    }
                else col--;
            }
    } else {
        cout << "File not able to be opened!!!" << endl;
        return false;
    }

    ///   Return   ///
    return !(maze_grid[0][0][0] != HALLWAY && numTimesRun <= 1 || maze_grid[X - 1][Y - 1][Z - 1] != HALLWAY);

}

bool Pathfinder::find_maze_path(int grid[X][Y][Z], int row, int col, int height){
    numTimesRun += 1;

    cout << this->toString();


    if (row < WALL || col < WALL || height < WALL || row >= X || col >= Y || height >= Z || grid[row][col][height] != HALLWAY) {
        return false;      // Cell is out of bounds or Cell is on barrier or dead end.
    } else if (row == X - 1 && col == Y - 1 && height == Z - 1) {
        grid[row][col][height] = PATH;         // Cell is on path
        solution.push_back("(" + to_string(height) + ", " + to_string(col) + ", " + to_string(row) + ")");
        return true;               // and is maze exit.
    } else {
        // Recursive case.
        // Attempt to find a path from each neighbor.
        // Tentatively mark cell as on path.
        grid[row][col][height] = PATH;

        if (find_maze_path(grid, row - 1, col, height) // Up
            || find_maze_path(grid, row + 1, col, height) // Down
            || find_maze_path(grid, row, col - 1, height) // Left
            || find_maze_path(grid, row, col + 1, height) // Right
            || find_maze_path(grid, row, col, height - 1) // In
            || find_maze_path(grid, row, col, height + 1)) // Out
        {
            solution.push_back("(" + to_string(height) + ", " + to_string(col) + ", " + to_string(row) + ")");
            return true;
        } else return false;
    }
}

vector<string> Pathfinder::solveMaze(){
    solution.clear();
    find_maze_path(maze_grid, 0,0,0);

    for(int row = 0; row < X; row++)
        for(int col = 0; col < Y; col++)
            for(int height = 0; height < Z; height++)
                maze_grid[row][col][height] = original_maze_grid[row][col][height];

    reverse(solution.begin(),solution.end());

    return solution;
}

