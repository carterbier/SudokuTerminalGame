#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "Sudoku.h"

bool Sudoku::isValid(const Board& board, int row, int column, int num) const
{
	int boxStartRow = (row / BOX_SIZE) * BOX_SIZE;
	int boxStartColumn = (column / BOX_SIZE) * BOX_SIZE;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (board[row][i] == num)
			return false;
	}
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (board[i][column] == num)
			return false;
	}
	for (int i = boxStartRow; i < boxStartRow + BOX_SIZE; i++)
	{
		for (int j = boxStartColumn; j < boxStartColumn + BOX_SIZE; j++)
		{
			if (board[i][j] == num)
				return false;
		}
	}

	return true;
}

int Sudoku::countSolutions(Board& board, int row, int column)
{
	if (row == BOARD_SIZE)
		return 1;
	if (column == BOARD_SIZE)
		return countSolutions(board, row + 1, 0);

	if (board[row][column] != EMPTY_CELL)
	{
		return countSolutions(board, row, column + 1);
	}
	int solutions = 0;
	for (int i = 1; i <= BOARD_SIZE; i++)
	{
		if (isValid(board, row, column, i))
		{
			board[row][column] = i;
			solutions += countSolutions(board, row, column + 1);
			board[row][column] = EMPTY_CELL;
			// Check if the puzzle has more than one solution
			if (solutions >= 2)
				return 2;
		}
	}
	return solutions;
}

bool Sudoku::fillBoard(Board& board, int row, int column, std::mt19937& generator)
{
	if (row == BOARD_SIZE)
		return true;
	if (column == BOARD_SIZE)
		return fillBoard(board, row + 1, 0, generator);

	int numbers[BOARD_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::shuffle(std::begin(numbers), std::end(numbers), generator);
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		int num = numbers[i];
		if (isValid(board, row, column, num))
		{
			board[row][column] = num;

			if (fillBoard(board, row, column + 1, generator))
				return true;

			board[row][column] = EMPTY_CELL;
		}
	}
	return false;
}

void Sudoku::generatePuzzle()
{
	int removed = 0;
	int positions[CELL_COUNT];

	std::random_device rd;
	std::mt19937 generator(rd());

	solution = {};
	fillBoard(solution, 0, 0, generator);
	puzzle = solution;

	for (int i = 0; i < CELL_COUNT; i++)
	{
		positions[i] = i;
	}
	std::shuffle(std::begin(positions), std::end(positions), generator);

	for (int i = 0; i < CELL_COUNT && removed < CELLS_TO_REMOVE; i++)
	{
		int row = positions[i] / BOARD_SIZE;
		int column = positions[i] % BOARD_SIZE;
		int temp = puzzle[row][column];
		puzzle[row][column] = EMPTY_CELL;
		int solutionCount = countSolutions(puzzle, 0, 0);
		if (solutionCount != 1)
		{
			puzzle[row][column] = temp;
		}
		else
			removed++;
	}
}

int Sudoku::getCell(int row, int column) const
{
	if (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE)
		throw std::out_of_range("Invalid Sudoku coordinates.");

	return puzzle[row][column];
}

Sudoku::MoveResult Sudoku::makeMove(int row, int column, int num)
{
	if (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE || num < 1 || num > BOARD_SIZE)
		return MoveResult::Invalid;
	if (puzzle[row][column] != EMPTY_CELL)
		return MoveResult::Occupied;
	if (num != solution[row][column])
		return MoveResult::Incorrect;

	puzzle[row][column] = num;
	return MoveResult::Success;
}

bool Sudoku::isComplete() const
{
	return puzzle == solution;
}