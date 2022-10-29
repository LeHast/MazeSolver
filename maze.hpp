#ifndef MAZE_HPP
#define MAZE_HPP

#include <vector>
#include <fstream>

// enums to represent the dirrection
enum class EDirection {
	eright, eleft, eup, edown, enone
};


class Maze {
public:
	Maze();
	void AddRow(std::vector<char> inRow);
	void FindPath();
private:
	std::vector<std::vector<char>> _mazeArray; // Original maze.
	std::vector<std::vector<char>> _finalMaze; // Final maze for output.
	std::vector<std::pair <int,int>> _cellStart; // Cell from the begining.
	int _row; // Number of rows.
	int _col; // Number of columns. 

	void FindStartEnd();
	void PrintFinalMaze();
	void SetStartEnd(int row, int col);
	void PushToStack(std::vector<std::vector<char>>& tempMaze, std::vector<std::pair<int, int>> &tempPath, EDirection &toChageDirection, EDirection direction, bool &bPop , int rpw, int col);
	void GetFinalMaze(std::vector<std::vector<char>> tempMaze);
	EDirection GetNeib(std::vector<std::vector<char>> inVect, int indexRow, int indexCol);

};


#endif 
//exit 0 - No file. 
//exit 2 - No exit found
//exit 3 - To many exits. 
//exit 4 - No path found.