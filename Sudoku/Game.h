#pragma once

#include "Sudoku.h"

#include <string>

class Game
{
public:
	void run();

private:
	static constexpr int ESC_KEY = 27;
	static constexpr int ENTER_KEY = 13;
	static constexpr int BACKSPACE_KEY = 8;

	Sudoku sudoku;

	void displayTitle();
	void displayMenu();
	void displayPuzzle();
	bool readGameInput(std::string& input);
	void playGame();
};