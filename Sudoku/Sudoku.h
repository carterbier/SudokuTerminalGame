#pragma once

#include <array>
#include <random>

class Sudoku
{
public:
	static constexpr int BOARD_SIZE = 9;
	static constexpr int BOX_SIZE = 3;
	static constexpr int EMPTY_CELL = 0;

	enum class MoveResult
	{
		Success, Occupied, Incorrect, Invalid
	};

	void generatePuzzle();
	int getCell(int row, int column) const;
	MoveResult makeMove(int row, int column, int num);
	bool isComplete() const;

private:
	static constexpr int CELL_COUNT = BOARD_SIZE * BOARD_SIZE;
	static constexpr int CELLS_TO_REMOVE = 30;

	using Board = std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE>;
	Board puzzle{};
	Board solution{};

	bool isValid(const Board& board, int row, int column, int num) const;
	int countSolutions(Board& board, int row, int column);
	bool fillBoard(Board& board, int row, int column, std::mt19937& generator);
};