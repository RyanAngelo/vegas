// Source file for Player
#include "Player.h"
#include <numeric>
#include <algorithm>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

void Player::rollAllDice() {
	cout << name << " rolls! ";
	for (auto& die : myDice) {
		if (!die.isPlayed()) {
			die.rollIt();
			cout << die.getValue() << " ";
		}
	}
	cout << std::endl;
}

void Player::addDiceToBoard(vector<Board>& boards, int boardId) {
	auto boardIt = std::find_if(boards.begin(), boards.end(),
		[boardId](const Board& board) { return board.getBoardId() == boardId; });
		
	if (boardIt != boards.end()) {
		for (auto& die : myDice) {
			if (die.getValue() == boardId && !die.isPlayed()) {
				boardIt->placeDie(die);
				die.setPlayed(true);
			}
		}
	}
}

bool Player::hasActiveDice() const {
	return std::any_of(myDice.begin(), myDice.end(),
		[](const Die& die) { return !die.isPlayed(); });
}

void Player::grabDice(int numberOfDice) {
	myDice.reserve(myDice.size() + numberOfDice);
	for (int i = 0; i < numberOfDice; ++i) {
		myDice.emplace_back(0, false);
		myDice.back().setOwner(shared_from_this());
	}
}

void Player::takeMoney(int moneyIn) {
	money.push_back(moneyIn);
}

int Player::getTotalMoney() const {
	return std::accumulate(money.begin(), money.end(), 0);
}

void Player::cleanupRound() {
	myDice.clear();
}