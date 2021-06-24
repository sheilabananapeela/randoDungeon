// Dungeon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <utility>
const int MAZESIZE = 50;

void printMaze(int grid[MAZESIZE][MAZESIZE]) {
    for (int i = 0; i < MAZESIZE; i++) {
        for (int j = 0; j < MAZESIZE; j++) {
            printf("%d", grid[i][j]);
        }
        printf("\n");
    }
}

std::pair<int, int> addPath(int x, int y, int dir) {
    int XX = x;
    int YY = y;
    if (dir == 0) { //go up
        XX = x;
        YY = y - 1;
    }
    else if (dir == 1) { //go down
        XX = x;
        YY = y + 1;
    }
    else if (dir == 2) { //go right
        XX = x + 1;
        YY = y;
    }
    else if(dir == 3) { //go left
        XX = x - 1;
        YY = y;
    }
    return std::make_pair(XX, YY);
}


//choose valid, avoid going out of bound, go new direction if already visited
std::pair<int, int> chooseDirection(int visited[MAZESIZE][MAZESIZE], int grid[MAZESIZE][MAZESIZE], int x, int y) {

    int XX = x;
    int YY = y;

    while (1) {
        bool valid = true;
        int dir = rand() % 4;

        std::pair<int, int> coord = addPath(x, y, dir);
        XX = coord.first;
        YY = coord.second;
        if (XX < 0 || XX >= MAZESIZE) {
            valid = false;
            continue;
        }

        if (YY < 0 || YY >= MAZESIZE) {
            valid = false;
            continue;
        }

        if (grid[XX][YY] == 7 || grid[XX][YY] == 8 || grid[XX][YY] == 3) {
            valid = false;
            continue;
        }

        //if (grid[XX][YY] == 1) {
            //valid = false;
            //continue;
        //}

        if (valid) {
            break;
        }
    }

    grid[XX][YY] = 1;
    return std::make_pair(XX, YY);
}

//generate the path towards to connect start and end
void generatePath(int visited[MAZESIZE][MAZESIZE], int grid[MAZESIZE][MAZESIZE], int startX, int startY, int endX, int endY) {
   //choose to go up, down, left, right?
    std::pair<int, int> coord1 = chooseDirection(visited, grid, startX, startY);
    std::pair<int, int> coord2 = chooseDirection(visited, grid, endX, endY);

    //end when the path connect
    if (coord1.first == coord2.first && coord1.second == coord2.second) {
        return;
    }

    //if (coord1.first == endX && coord1.second == endY) {
    //    return;
    //}

    generatePath(visited, grid, coord1.first, coord1.second, coord2.first, coord2.second);
    //generatePath(visited, grid, startX, startY, coord2.first, coord2.second);
    //generatePath(visited, grid, coord1.first, coord1.second, endX, endY);
}

void helper(int grid[MAZESIZE][MAZESIZE], int x, int y) {
    if (x >= MAZESIZE || x < 0) {
        return;
    }
    if (y >= MAZESIZE || y < 0) {
        return;
    }
    if (grid[x][y] == 1 || grid[x][y] == 7 || grid[x][y] == 8 || grid[x][y] == 3) {
        return;
    }

    grid[x][y] = 1;

    int rando = rand() % 3;

    if (rando == 0) {
        if (x >= MAZESIZE / 2) { //prefer to go right
            helper(grid, x + 1, y);
        }
        else { //prefer to go left
            helper(grid, x - 1, y);
        }
    }
    else if(rando == 1) {
        if (y >= MAZESIZE / 2) { //prefer to go up
            helper(grid, x, y -1);
        }
        else { //prefer to go down
            helper(grid, x, y+1);
        }
    }
    else {
        helper(grid, x + 1, y);
        helper(grid, x - 1, y);
        helper(grid, x, y - 1);
        helper(grid, x, y + 1);
    }
}

void makeRoom(int grid[MAZESIZE][MAZESIZE], int roomNum) {
    int count = 0;
    while(count < roomNum) {
        int XX = rand() % MAZESIZE;
        int YY = rand() % MAZESIZE;

        if (grid[XX][YY] == 3 || grid[XX][YY] == 7 || grid[XX][YY] == 8) {
            continue;
        }

        grid[XX][YY] = 3;
        count++;

        //try to connect room
        helper(grid, XX+1, YY);
        helper(grid, XX-1, YY);
        helper(grid, XX, YY+1);
        helper(grid, XX, YY-1);
    }
}


int main()
{
    std::cout << "Generating dungeon maze\n";
    std::cout << "Random rooms will be denoted by 3\n";
    srand(time(NULL));

    int mazeSize = 10;

    int maxRoom = 10;
    
    int roomNum = rand() % maxRoom;

    int grid[MAZESIZE][MAZESIZE] = { 0 };
    int visited[MAZESIZE][MAZESIZE] = { 0 };

    printf("roomNum: %d\n", roomNum);

    //grid[2][2] = 1;
    //printMaze(grid);

    //get the random location of Start and End
    int startX = rand() % MAZESIZE;
    int startY = rand() % MAZESIZE;

    int endX = rand() % MAZESIZE;
    int endY = rand() % MAZESIZE;
    while (endY == startY && startX == endX) {
        endY = rand() % MAZESIZE;
    }

    printf("startCoord: %d, %d\n", startX, startY);
    printf("endCoord: %d, %d\n", endX, endY);

    grid[startX][startY] = 3;
    grid[endX][endY] = 3;

    helper(grid, startX + 1, startY);
    helper(grid, startX, startY+1);
    helper(grid, startX - 1, startY);
    helper(grid, startX, startY - 1);

    helper(grid, endX + 1, endY);
    helper(grid, endX, endY + 1);
    helper(grid, endX - 1, endY);
    helper(grid, endX, endY - 1);

    //printMaze(grid);
    //generatePath(visited, grid, startX, startY, endX, endY);

    //randomly place R (ignore location S, E) Try not to get rooms to overlap each other, or the start, end
    //connect them too
    makeRoom(grid, roomNum);

    printMaze(grid);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
