#include <iostream>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <limits>

void displayPuzzle(int board[][9])
{
	std::cout << "\n";
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] == 0)
				std::cout << " " << " ";
			else
				std::cout << board[i][j] << " ";

			if (j == 2 || j == 5)
				std::cout << "| ";
		}
		std::cout << "\n";

		if (i == 2 || i == 5)
			std::cout << "------+-------+------\n";
	}
	std::cout << "\n";
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

void playGame(int puzzle[][9], int solution[][9])
{
	int row, column, num;

	std::string message = "";

	while (!isComplete(puzzle, solution))
	{
		system("cls");
		std::cout << message << "\n";
		displayPuzzle(puzzle);

		std::string input;
		std::cout << "Enter row: ";
		if (!std::getline(std::cin, input))
			return;
		if (!isValidInput(input))
		{
			message = "Invalid input. Enter values from 1 to 9.";
			continue;
		}
		row = input[0] - '0';

		std::cout << "Enter column: ";
		if (!std::getline(std::cin, input))
			return;
		if (!isValidInput(input))
		{
			message = "Invalid input. Enter values from 1 to 9.";
			continue;
		}
		column = input[0] - '0';

		std::cout << "Enter number: ";
		if (!std::getline(std::cin, input))
			return;
		if (!isValidInput(input))
		{
			message = "Invalid input. Enter values from 1 to 9.";
			continue;
		}
		num = input[0] - '0';

		row--;
		column--;

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
	displayPuzzle(puzzle);
	std::cout << "\nCongratulations! You solved the puzzle!\n";

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

int main()
{
	char input;

	std::cout << "Would you like to play Sudoku (Y/n): ";
	if (!(std::cin >> input))
		return 0;
	
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	if (input == 'Y' || input == 'y')
		generatePuzzle();

	return 0;
}