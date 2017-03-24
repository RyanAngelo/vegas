// Source file for Player
#include "Player.h"

using std::vector;
using std::cout;
using std::endl;

void Player::rollAllDice(){
	cout << name << " rolls! ";
	for(vector<Die>::iterator it = myDice.begin(); it != myDice.end(); ++it){
		Die& die = *it;
		if(die.isPlayed()==false){ 
			die.rollIt();
			cout << die.getValue() << " ";
		}
	}
	cout << std::endl;
}

void Player::addDiceToBoard(vector<Board>& boards, int boardId){
	//dice iterator
	vector<Die>::iterator dIt;
	//board iterator
	vector<Board>::iterator bIt;

	for (bIt = boards.begin(); bIt != boards.end(); ++bIt) {
		if( bIt->getBoardId()==boardId){
			Board& board = *bIt;
			for (dIt = myDice.begin(); dIt != myDice.end(); ++dIt) {
				Die& die = *dIt;
				if( die.getValue()==board.getBoardId() && die.isPlayed()==false){
					bIt->placeDie(die);
					die.setPlayed(true);
				}
			}
		}
	}
}

bool Player::hasActiveDice(){
	for(vector<Die>::iterator it = myDice.begin(); it != myDice.end(); ++it){
		Die& die = *it;
		if(die.isPlayed()==false){ 
			return true;
		}
	}
	return false;
}

void Player::grabDice(int numberOfDice){
	//generate x number of dice
	for (int i=0; i<numberOfDice; ++i) {
		//Declare value on the heap to persist
		Die* newDie = new Die(0,false);
		newDie->setOwner(this);
	    myDice.push_back(*newDie);
	}
}

void Player::takeMoney(int moneyIn) {
	money.push_back(moneyIn);
}

vector<Die> Player::getAllDice(){
	return myDice;
}

string Player::getPlayerName(){
	return name;
}

int Player::getPlayerIdentifier(){
	return identifier;
}

int Player::getTotalMoney(){
	vector<int>::iterator mIt;
	int total=0;
	for(mIt = money.begin(); mIt != money.end(); ++mIt){
		total=*mIt+total;
	}
	return total;
}