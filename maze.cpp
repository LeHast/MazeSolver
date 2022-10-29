#include "maze.hpp"
#include <iostream>

Maze::Maze(){
	_col = 0;
	_row = 0;
}

/// This function accepts char vector from the main function that represent a row.
/// Then add to row to the maze.
void Maze::AddRow(std::vector<char> inRow){
	_mazeArray.push_back(inRow);
	_col = inRow.size();
	_row++;
}

/// This function checks every character in the first and last rows/cols for '0's.
void Maze::FindStartEnd(){
	//Scan firt row.
	for (int colCount = 1; colCount < _col - 1; colCount++) {
		if (_mazeArray[0][colCount] == '0') {
			SetStartEnd(0, colCount);
		}
	}

	//Scan firt column.
	for (int rowCount = 0; rowCount < _row; rowCount++) {
		if (_mazeArray[rowCount][0] == '0') {
			SetStartEnd(rowCount, 0);
		}
	}

	//Scan last row.
	for (int colCount = 1; colCount < _col - 1; colCount++) {
		if (_mazeArray[_row - 1][colCount] == '0') {
			SetStartEnd(_row - 1, colCount);
		}
	}

	//Scan last column.
	for (int rowCount = 0; rowCount < _row; rowCount++) {
		if (_mazeArray[rowCount][_col - 1] == '0') {
			SetStartEnd(rowCount, _col - 1);
		}
	}

	// If the start or exit are not found, the program will exit.
	if (_cellStart.size() < 2) {
		std::cout << " No exit found"  << std::endl;
		exit(2);
	}
}

/// It accepts the two intgers, the X (inCol) and Y (inRow) to set the the start and the end points.
/// Add two cell to the stack to represent the start and the end.
void Maze::SetStartEnd(int inRow, int inCol){
	if (_cellStart.size() < 2 ) {
		_cellStart.push_back(std::make_pair(inRow, inCol));	
	}
	else {
		std::cout << "To many exits!" << std::endl; 
		exit(3);
	}
}

/// It needs a maze(_mazeArray) to work.
/// It will find a path.
/// If there is not path, it will exit.
void Maze::FindPath() {
	FindStartEnd();

	int rowCount = _cellStart[0].first; // Cell X and Y of the movement.
	int colCount = _cellStart[0].second;

	const int startX = _cellStart[0].first;	// Start Cell.
	const int startY = _cellStart[0].second;

	const int endX = _cellStart[1].first;	// End cell.
	const int endY = _cellStart[1].second;

	EDirection tempDirection = EDirection::enone; // Direction that the path is going.

	std::vector<std::pair<int, int>> tempPath;
	std::vector<std::vector<char>> tempMaze = _mazeArray; // Make a copy from the original maze.
	tempPath.push_back(std::make_pair(rowCount, colCount)); // Push the start Cell
	tempMaze[tempPath[tempPath.size() - 1].first][tempPath[tempPath.size() - 1].second] = '2'; // Changing the start cell of the path as view;

	bool endFound = false;
	while (!endFound) {
		bool bPop = false;

		//Going Right.
		while (colCount < _col - 1) {
			if (rowCount == endX && colCount == endY) { // if path was found break
				endFound = true;
				break;
			}
			if (tempMaze[rowCount][colCount + 1] == '1' || tempMaze[rowCount][colCount + 1] == '2') { // If hits a wall of a visited cell break
				break;
			}
			colCount++;
			PushToStack(tempMaze, tempPath, tempDirection, EDirection::eright, bPop, rowCount, colCount); // Push the cell to the stack
		}

		//Going Down.
		while (rowCount + 1 < _row) {
			if (rowCount == endX && colCount == endY) {
				endFound = true;
				break;
			}
			if (tempMaze[rowCount + 1][colCount] == '1' || tempMaze[rowCount + 1][colCount] == '2') { 
				break; //leave going to rigth
			}
			rowCount++;
			PushToStack(tempMaze, tempPath, tempDirection, EDirection::edown, bPop, rowCount, colCount);
		}

		//Going left.
		while (colCount - 1 >= 0 ) {
			if (rowCount == endX && colCount == endY) {
				endFound = true;
				break;
			}
			if (tempMaze[rowCount][colCount - 1] == '1' || tempMaze[rowCount][colCount - 1] == '2') {
				break;
			}
			colCount--;
			PushToStack(tempMaze, tempPath, tempDirection, EDirection::eleft, bPop, rowCount, colCount);
		}

		//Going UP.
		while (rowCount - 1 < _row && rowCount - 1 >= 0) {
			if (rowCount == endX && colCount == endY) {
				endFound = true;
				break;
			}
			if (tempMaze[rowCount - 1][colCount] == '1' || tempMaze[rowCount - 1][colCount] == '2') { // if going right hit a wall
				break;
			}
			rowCount--;
			PushToStack(tempMaze, tempPath, tempDirection, EDirection::eup, bPop, rowCount, colCount);
		}

		/// Pop the last cell in the stack
		if (!bPop && !endFound) {
			tempDirection = GetNeib(tempMaze, tempPath[tempPath.size() - 1].first, tempPath[tempPath.size() - 1].second); // Get neighbors.
			tempMaze[tempPath[tempPath.size() - 1].first][tempPath[tempPath.size() - 1].second] = '1'; // Change last Cell of the stack for a wall.
			tempPath.pop_back(); // Delete the last cell from the path.
			bPop = false;
			if (tempDirection == EDirection::enone || (rowCount == startX && colCount == startY)) { //if there is not path exit the program
				std::cout << "No path found " << std::endl;
				exit(4);
			}
			else { //Fix last location of the cell's path
				rowCount = tempPath[tempPath.size() - 1].first;
				colCount = tempPath[tempPath.size() - 1].second;
			}
		}
	}
	GetFinalMaze(tempMaze);
	PrintFinalMaze(); 
}

/// It accetps a 2D vector for temp maze(tempMaze), pair vector for the path (tempPath), Enum EDirection for the current location(toChageDirection),
/// Enum EDirection  to change the current location(direction), bool to disable the pop stack for until it hits a wall (bPop), integer for the row number(row)
/// and an integer for the row colunm (col). 
///	Push the cell into the stack.
/// It change the direction to the way is going
/// And it changes the cell into a visited cell '2'
void Maze::PushToStack(std::vector<std::vector<char>>& tempMaze, std::vector<std::pair<int, int>>& tempPath, EDirection& toChageDirection,EDirection direction, bool& bPop, int row, int col){
	tempPath.push_back(std::make_pair(row, col)); //push X and Y to the location .
	toChageDirection = direction; // it is going down.
	bPop = true;
	tempMaze[tempPath[tempPath.size() - 1].first][tempPath[tempPath.size() - 1].second] = '2';

}

/// Accepts a 2D vetor for the current maze (inVect), integer for the row number (indexRow), and integer for the column number (indexCol).
/// Return the next neighbor.
EDirection Maze::GetNeib(std::vector<std::vector<char>> inVect, int indexRow, int indexCol){
	if (indexRow > 0)
		if (inVect[indexRow - 1][indexCol] != '0' ) {
			return EDirection::eup;
		}
	if (indexRow < _row)
		if (inVect[indexRow + 1][indexCol] != '0') {
			return EDirection::edown;
		}
	if (indexCol < _col)
		if (inVect[indexRow][indexCol + 1] != '0') {
			return EDirection::eright;
		}
	if (indexCol > 0)
		if (inVect[indexRow][indexCol - 1] != '0') {
			return EDirection::eleft;
		}
	return EDirection::enone;
}

// Accepts a 2D vhar vector (tempMaze).
// Adds the path to the final maze and then print final maze.
void Maze::GetFinalMaze(std::vector<std::vector<char>> tempMaze){
	std::vector<char> toFinalMaze;

	// Adds the path to the final maze and then print final maze.
	for (int rowCout = 0; rowCout < _row; rowCout++) {
		for (int colCount = 0; colCount < _col; colCount++) {
			if (tempMaze[rowCout][colCount] == '2')
				toFinalMaze.push_back(' ');
			else
				toFinalMaze.push_back(_mazeArray[rowCout][colCount]);
		}
		_finalMaze.push_back(toFinalMaze);
		toFinalMaze.clear();
	}

}

/// Print output.
void Maze::PrintFinalMaze(){
	for (int rowCout = 0; rowCout < _row; rowCout++) {
		for (int colCount = 0; colCount < _col; colCount++) {
			std::cout << _finalMaze[rowCout][colCount];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}