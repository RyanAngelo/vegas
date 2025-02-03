//board.cc
#include "Board.h"
#include <algorithm>
#include <numeric>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

int Board::placeDie(Die& playerDie) {
	playerDie.setPlayed(true);
	dice.push_back(playerDie);
	return dice.size();
}

void Board::placeMoney() {
	std::random_device seed;
	std::mt19937 gen(seed());
	std::uniform_int_distribution<int> dist(1, 9);
	money.push_back(dist(gen) * 10000);
}

int Board::getTotalMoney() const {
	return std::accumulate(money.begin(), money.end(), 0);
}

void Board::printMoney() const {
	for (const auto& amount : money) {
		std::cout << " $" << amount;
	}
}

int Board::takeLargestBill() {
	if (money.empty()) {
		return 0;
	}
	
	auto maxIt = std::max_element(money.begin(), money.end());
	int maxValue = *maxIt;
	money.erase(maxIt);
	return maxValue;
}

void Board::cleanupRound() {
	money.clear();
	dice.clear();
}