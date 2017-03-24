//Die.h
#ifndef __DIE_H_INCLUDED__
#define __DIE_H_INCLUDED__

#include <vector>
#include <random>
#include <string>
#include <iostream>

class Player;

class Die {
private:
	int value;
	bool played;
	Player *owner;

public:
	
	Die(int val=0, bool playedState=false){
		value=val;
		played=playedState;
	}
	//randomized roll, changes single die value value
	int rollIt();
	bool isPlayed();
	void setPlayed(bool);
	int getValue() const;
	class Player* getOwner();
	void setOwner(class Player*);
	void printDieAscii();
};

#endif