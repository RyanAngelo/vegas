//board.cc
#include "Board.h"

using std::cout;
using std::endl;
using std::vector;

int Board::totalDice() const{
	return dice.size();
}

vector<Die> Board::getDice() {		
	return dice;
}

int Board::placeDie(Die& playerDie){
	playerDie.setPlayed(true);
	dice.push_back(playerDie);
	return dice.size();
}

void Board::placeMoney(){
	int moneyValue=0;
	std::random_device seed;
	std::mt19937 gen(seed());
	std::uniform_int_distribution<int> dist(1, 9);
	moneyValue=dist(gen);
	money.push_back(moneyValue*10000);
}

int Board::getBoardId(){
	return boardId;
}

int Board::getTotalMoney(){
	int total=0;
	vector<int>::iterator mIt;
	for (mIt = money.begin(); mIt != money.end(); ++mIt) {
		total=total+*mIt;
	}
	return total;
}

void Board::printMoney(){
	vector<int>::iterator mIt;
	for (mIt = money.begin(); mIt != money.end(); ++mIt) {
		cout << " $" << *mIt;
	}
}

int Board::takeLargestBill(){
	vector<int>::iterator mIt;
	mIt = max_element(money.begin(), money.end());
	if(mIt!=money.end()){
		int maxValue = *mIt;
		mIt = money.erase(mIt);
		return maxValue;
	}
	return 0;
}

vector<int> Board::getMoney(){
	return money;
}

void Board::cleanupRound(){
	money.clear();
	dice.clear();
}