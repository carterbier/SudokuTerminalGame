#include <iostream>
#include <random>
#include <algorithm>

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
	for (int i = boxStartRow; i < boxStartRow+3; i++)
	{
		for (int j = boxStartColumn; j < boxStartColumn+3; j++)
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

bool fillBoard(int board[][9], int row, int column, std::mt19937& generator)
{
	int numbers[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::shuffle(std::begin(numbers), std::end(numbers), generator);	//randomizes the order to generate a different valid board


	if (row == 9)	//The board has been filled
		return true;
	if (column == 9)
		return fillBoard(board, row + 1, 0, generator);

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

void playGame(int puzzle[][9], int solution[][9])
{
	int row, column, num;

	while (!isComplete(puzzle, solution))
	{
		displayPuzzle(puzzle);

		std::cout << "Enter row: ";
		std::cin >> row;

		std::cout << "Enter column: ";
		std::cin >> column;

		std::cout << "Enter number: ";
		std::cin >> num;


		if (row < 1 || row > 9 ||
			column < 1 || column > 9 ||
			num < 1 || num > 9)
			std::cout << "\n//Input is invalid//\n";
		else
		{
			row--;
			column--;

			if (puzzle[row][column] != 0)
				std::cout << "\n//The cell is already filled//\n";
			else if (num != solution[row][column])
				std::cout << "\n//Incorrect number//\n";
			else
				puzzle[row][column] = num;
		}
	}

	displayPuzzle(puzzle);
	std::cout << "\nYou completed the puzzle!\n";

}

void generatePuzzle()
{
	int board[9][9] = {};
	int puzzle[9][9];
	int removed = 0;

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

	std::uniform_int_distribution<int> indexDistribution(0, 8);

	while (removed < 3)
	{
		int row = indexDistribution(generator);
		int column = indexDistribution(generator);

		if (puzzle[row][column] != 0)
		{
			puzzle[row][column] = 0;
			removed++;
		}
	}

	playGame(puzzle, board);
	
}

int main()
{
	char input;
	
	std::cout << "Would you like to play Sudoku (Y/n): ";
	std::cin >> input;

	if (input == 'Y' || input == 'y')
		generatePuzzle();
	else
		return 0;
	
	return 0;
}