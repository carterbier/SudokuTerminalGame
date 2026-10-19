#include <iostream>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <conio.h>

void displayPuzzle(int board[][9])
{
	std::cout << "\t     ";
	for (int i = 0; i < 9; i++)
	{
		std::cout << i + 1 << " ";
		if (i == 2 || i == 5)
			std::cout << "  ";
	}
	std::cout << "\n\t   +-------+-------+-------+\n";
	for (int i = 0; i < 9; i++)
	{
		std::cout << "\t " << i + 1 << " | ";
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] == 0)
				std::cout << "  ";
			else
				std::cout << board[i][j] << " ";

			if (j == 2 || j == 5)
				std::cout << "| ";
		}
		std::cout << "|\n";

		if (i == 2 || i == 5)
			std::cout << "\t   +-------+-------+-------+\n";
	}
	std::cout << "\t   +-------+-------+-------+\n\n";
}

bool isValid(int board[][9], int row, int column, int num)	//checks all three Sudoku constraints
{
	int boxStartRow = (row / 3) * 3;
	int boxStartColumn = (column / 3) * 3;

	//row
	for (int i = 0; i < 9; i++)
	{
		if (board[row][i] == num)
			return false;
	}
	//column
	for (int i = 0; i < 9; i++)
	{
		if (board[i][column] == num)
			return false;
	}
	//box
	for (int i = boxStartRow; i < boxStartRow + 3; i++)
	{
		for (int j = boxStartColumn; j < boxStartColumn + 3; j++)
		{
			if (board[i][j] == num)
				return false;
		}
	}


	return true;
}

bool isComplete(int puzzle[][9], int solution[][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (puzzle[i][j] != solution[i][j])
				return false;
		}
	}
	return true;
}

int countSolutions(int board[][9], int row, int column)
{
	if (row == 9)
		return 1;
	if (column == 9)
		return countSolutions(board, row + 1, 0);

	if (board[row][column] != 0)
	{
		return countSolutions(board, row, column + 1);
	}

	int solutions = 0;
	for (int i = 1; i <= 9; i++)
	{
		if (isValid(board, row, column, i))
		{
			board[row][column] = i;
			solutions += countSolutions(board, row, column + 1);
			board[row][column] = 0;
			if (solutions >= 2)
				return 2;
		}
	}
	return solutions;
}

bool fillBoard(int board[][9], int row, int column, std::mt19937& generator)
{
	int numbers[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	if (row == 9)	//The board has been filled
		return true;
	if (column == 9)
		return fillBoard(board, row + 1, 0, generator);

	std::shuffle(std::begin(numbers), std::end(numbers), generator);	//randomizes the order to generate a different valid board

	for (int i = 0; i < 9; i++)
	{
		int num = numbers[i];

		if (isValid(board, row, column, num))
		{
			board[row][column] = num;

			if (fillBoard(board, row, column + 1, generator))
				return true;

			board[row][column] = 0;	//dead end
		}
	}

	return false;
}

bool readGameInput(std::string& input)
{
	input.clear();

	while (true)
	{
		int key = _getch();

		if (key == 27)
			return false;
		if (key == 13)
		{
			std::cout << "\n";
			return true;
		}
		if (key >= '1' && key <= '9' && input.empty())
		{
			input += static_cast<char>(key);
			std::cout << static_cast<char>(key);
		}
		if (key == 8)
		{
			if (!input.empty())
			{
				input.pop_back();
				std::cout << "\b \b";
			}
		}
		if (key == 0 || key == 224)
		{
			(void)_getch();
			continue;
		}
	}
}

bool isValidInput(const std::string& input)
{
	if (input.length() != 1)
	{
		return false;
	}
	if (input[0] < '1' || input[0] > '9')
	{
		return false;
	}
	return true;
}

void displayTitle()
{
	std::cout << "\n================================================";
	std::cout << "\n            SUDOKU C++ Terminal Game            ";
	std::cout << "\n================================================\n\n";
}

void playGame(int puzzle[][9], int solution[][9])
{
	int row, column, num;

	std::string message = "";
	const std::string prompts[3] = { "Enter row: ", "Enter column: ", "Enter number: " };

	while (!isComplete(puzzle, solution))
	{
		system("cls");
		std::cout << " [ESC] Main Menu";
		displayTitle();
		displayPuzzle(puzzle);
		std::cout << message << "\n\n";

		std::string inputs[3];

		bool valid = true;

		for (int i = 0; i < 3; i++)
		{
			std::cout << prompts[i];

			if (!readGameInput(inputs[i]))
				return;

			if (!isValidInput(inputs[i]))
			{
				message = "Invalid input. Enter values from 1 to 9.";
				valid = false;
				break;
			}

		}

		if (!valid)
			continue;
		
		row = inputs[0][0] - '1';
		column = inputs[1][0] - '1';
		num = inputs[2][0] - '0';

		if (puzzle[row][column] != 0)
			message = "The cell is already filled.";
		else if (num != solution[row][column])
			message = "Incorrect number. Please try again.";
		else
		{
			puzzle[row][column] = num;
			message = "";
		}
	}
	system("cls");
	displayTitle();
	displayPuzzle(puzzle);
	std::cout << "================================================";
	std::cout << "\n    Congratulations! You solved the puzzle!";
	std::cout << "\n================================================\n";
	std::cout << "\nPress [ESC] to return to the main menu...";
	while (_getch() != 27);
}

void generatePuzzle()
{
	int board[9][9] = {};
	int puzzle[9][9];
	int removed = 0;
	int positions[81];

	std::random_device rd;
	std::mt19937 generator(rd());

	fillBoard(board, 0, 0, generator);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			puzzle[i][j] = board[i][j];
		}
	}

	for (int i = 0; i < 81; i++)
	{
		positions[i] = i;
	}
	std::shuffle(std::begin(positions), std::end(positions), generator);

	for (int i = 0; i < 81 && removed < 30; i++)
	{
		int row = positions[i] / 9;
		int column = positions[i] % 9;

		if (puzzle[row][column] != 0)
		{
			int temp = puzzle[row][column];
			puzzle[row][column] = 0;
			int solutionCount = countSolutions(puzzle, 0, 0);
			if (solutionCount != 1)
			{
				puzzle[row][column] = temp;
			}
			else
				removed++;
		}
	}
	playGame(puzzle, board);
}

void displayMenu()
{
	while (true) 
	{
		system("cls");
		std::cout << "[ESC] Exit Game";
		displayTitle();
		std::cout << "\n\t[ENTER] Play Game\n\n";
		
		int key = _getch();

		if (key == 13)
			generatePuzzle();
		else if (key == 27)
		{
			system("cls");
			displayTitle();
			std::cout << "\n\tThanks for playing...\n\n";
			return;
		}
	}
}

int main()
{
	displayMenu();

	return 0;
}