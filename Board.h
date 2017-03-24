// Board.h
#ifndef __BOARD_H_INCLUDED__
#define __BOARD_H_INCLUDED__

#include <vector>
#include "Die.h"

class Board
{
private:
	int boardId;
	std::vector<Die> dice;
	std::vector<int> money;
	
public:
	
	Board(int bId=0){
		boardId=bId;
	}
	
	//sortMoney(money)
	void sortMoney(std::vector<int>&); //Sort money from highest to lowest
	//Place a single die on the board
	int placeDie(Die&);
	void placeMoney();
	int totalDice() const;
	std::vector<Die> getDice();
	int getBoardId();
	int getTotalMoney();
	void printMoney();
	int takeLargestBill();
	std::vector<int> getMoney();
};

#endif