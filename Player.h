// Board.h
#ifndef __PLAYER_H_INCLUDED__
#define __PLAYER_H_INCLUDED__

#include <vector>
#include <string>
#include <iostream>
#include "Die.h"
#include "Board.h"

using std::string;

class Player {
private:
	std::string name;
	int identifier;
	std::vector<int> money;
	std::vector<Die> myDice;
	
public:
	
	Player(std::string playerName="Player", int identifierNum=69){
		name=playerName;
		identifier=identifierNum;
	}
	
	//Generate a hand of dice
	void grabDice(int);
	//Add dice to a board
	void addDiceToBoard(std::vector<Board>&, int);
	//Roll all dice, giving all ACTIVE dice new values
	void rollAllDice();
	//Return whether the player has active dice
	bool hasActiveDice();
	//Place chosen dice on selected board
	void placeChosenDice(int);
	//Get all dice associated with the player
	std::vector<Die> getAllDice();
	string getPlayerName();
	int getPlayerIdentifier();
	//take money and add it to the existing player money
	void takeMoney(int);
	//Get the total amount of money won by player
	int getTotalMoney();
	void cleanupRound();
};

#endif