// Daniel Galarza 
// CS 315
// Project 1

#include <iostream>
#include <fstream>
#include <vector>
#include "maze.hpp"

int main(int argc, char* argv[]) {
	//This program needs two arguments to run.
	if (argc != 2) {
		std::cout << "No 2 arguments" << std::endl;
		exit(1);
	}

	std::fstream inFile;

	inFile.open(argv[1], std::ios::in);

	// If the file is not open then exit
	if (!inFile.is_open()) {
		std::cout << "No file" << std::endl;
		exit(0);
	}	

	Maze maze;
	char r;
	std::vector<char> vectemp;

	// Read the file then add the rows to the maze
	while (inFile >> r) {
		vectemp.push_back(r);
		if (inFile.peek() == '\n' || inFile.eof() || inFile.peek() == '\r') {
			maze.AddRow(vectemp);
			vectemp.clear();
		}
	}
	inFile.close();

	maze.FindPath();

	return 0;
}