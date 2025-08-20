// Player.h
#ifndef VEGAS_PLAYER_H
#define VEGAS_PLAYER_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "Die.h"
#include "Board.h"

class Player : public std::enable_shared_from_this<Player> {
private:
	std::string name;
	int identifier;
	std::vector<int> money;
	std::vector<Die> myDice;
	
public:
	
	explicit Player(std::string playerName = "Player", int identifierNum = 1)
		: name(std::move(playerName))
		, identifier(identifierNum) {}

	// Rule of five - explicitly default these
	Player(const Player&) = default;
	Player& operator=(const Player&) = default;
	Player(Player&&) noexcept = default;
	Player& operator=(Player&&) noexcept = default;
	~Player() = default;

	// Dice management
	void grabDice(int numberOfDice);
	void addDiceToBoard(std::vector<Board>& boards, int boardId);
	void rollAllDice();
	bool hasActiveDice() const;
	std::vector<int> getAvailableDiceValues() const;
	const std::vector<Die>& getAllDice() const { return myDice; }

	// Player information
	const std::string& getPlayerName() const { return name; }
	int getPlayerIdentifier() const { return identifier; }

	// Money management
	void takeMoney(int moneyIn);
	int getTotalMoney() const;
	void cleanupRound();
};

#endif