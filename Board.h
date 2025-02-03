// Board.h
#ifndef VEGAS_BOARD_H
#define VEGAS_BOARD_H

#include <vector>
#include <algorithm>
#include "Die.h"

class Board {
private:
	int boardId;
	std::vector<Die> dice;
	std::vector<int> money;
	
public:
	
	explicit Board(int bId = 0) 
		: boardId(bId) {}

	// Rule of five - explicitly default these
	Board(const Board&) = default;
	Board& operator=(const Board&) = default;
	Board(Board&&) noexcept = default;
	Board& operator=(Board&&) noexcept = default;
	~Board() = default;

	// Place a single die on the board
	int placeDie(Die& playerDie);
	void placeMoney();
	int totalDice() const { return dice.size(); }
	const std::vector<Die>& getDice() const { return dice; }
	int getBoardId() const { return boardId; }
	int getTotalMoney() const;
	void printMoney() const;
	int takeLargestBill();
	const std::vector<int>& getMoney() const { return money; }
	void cleanupRound();
};

#endif