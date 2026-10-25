#include <conio.h>
#include <iostream>

#include "Game.h"
#include "Terminal.h"

void Game::run()
{
	displayMenu();
}

void Game::displayTitle()
{
	std::cout << "\n=================================================";
	std::cout << "\n            SUDOKU C++ Terminal Game";
	std::cout << "\n=================================================\n\n";
}

void Game::displayMenu()
{
	while (true)
	{
		Terminal::clearScreen();
		std::cout << " [ESC] Exit Game";
		displayTitle();
		std::cout << "\n\t\t[ENTER] Play Game\n\n";

		int key;
		do 
			key = _getch();
		while (key != ENTER_KEY && key != ESC_KEY);

		if (key == ENTER_KEY)
		{
			sudoku.generatePuzzle();
			playGame();
		}
		else if (key == ESC_KEY)
		{
			return;
		}
	}
}

void Game::displayPuzzle()
{
	std::cout << "\t     ";
	for (int i = 0; i < Sudoku::BOARD_SIZE; i++)
	{
		std::cout << i + 1 << " ";
		if (i == 2 || i == 5)
			std::cout << "  ";
	}
	std::cout << "\n\t   +-------+-------+-------+\n";
	for (int i = 0; i < Sudoku::BOARD_SIZE; i++)
	{
		std::cout << "\t " << i + 1 << " | ";
		for (int j = 0; j < Sudoku::BOARD_SIZE; j++)
		{
			int cell = sudoku.getCell(i, j);
			if (cell == Sudoku::EMPTY_CELL)
				std::cout << "  ";
			else
				std::cout << cell << " ";

			if (j == 2 || j == 5)
				std::cout << "| ";
		}
		std::cout << "|\n";

		if (i == 2 || i == 5)
			std::cout << "\t   +-------+-------+-------+\n";
	}
	std::cout << "\t   +-------+-------+-------+\n\n";
}

bool Game::readGameInput(std::string& input)
{
	input.clear();
	while (true)
	{
		int key = _getch();
		if (key == ESC_KEY)
			return false;
		if (key == ENTER_KEY)
		{
			std::cout << "\n";
			return true;
		}
		// Accepts only a single digit
		if (key >= '1' && key <= '9' && input.empty())
		{
			input += static_cast<char>(key);
			std::cout << static_cast<char>(key);
		}
		if (key == BACKSPACE_KEY && !input.empty())
		{
			input.pop_back();
			std::cout << "\b \b";
		}
		// Ignore function keys, arrow keys, and other navigation keys.
		if (key == 0 || key == 224)
		{
			(void)_getch();
			continue;
		}
	}
}

void Game::playGame()
{
	std::string message;
	const std::string prompts[3] = { "Enter row: ", "Enter column: ", "Enter number: " };

	while (!sudoku.isComplete())
	{
		Terminal::clearScreen();
		std::cout << " [ESC] Main Menu";
		displayTitle();
		displayPuzzle();

		if (!message.empty())
			std::cout << "\t[!] " << message;
		std::cout << "\n\n";

		std::string inputs[3];
		bool valid = true;
		for (int i = 0; i < 3; i++)
		{
			std::cout << prompts[i];
			if (!readGameInput(inputs[i]))
				return;
			if (inputs[i].empty())
			{
				message = "Invalid input. Enter values from 1 to 9.";
				valid = false;
				break;
			}
		}

		if (!valid)
			continue;

		const int row = inputs[0][0] - '1';
		const int column = inputs[1][0] - '1';
		const int num = inputs[2][0] - '0';

		Sudoku::MoveResult result = sudoku.makeMove(row, column, num);

		if (result == Sudoku::MoveResult::Success)
			message = "";
		else if (result == Sudoku::MoveResult::Occupied)
			message = "The cell is already filled.";
		else if (result == Sudoku::MoveResult::Incorrect)
			message = "Incorrect number. Please try again.";
		else if (result == Sudoku::MoveResult::Invalid)
			message = "Invalid move.";
	}
	Terminal::clearScreen();
	std::cout << " [ESC] Main Menu";
	displayTitle();
	displayPuzzle();
	std::cout << "=================================================";
	std::cout << "\n    Congratulations! You solved the puzzle!";
	std::cout << "\n=================================================\n";
	while (_getch() != ESC_KEY);
}