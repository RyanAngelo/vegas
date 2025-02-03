// Die.h
#ifndef VEGAS_DIE_H
#define VEGAS_DIE_H

#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <memory>

class Player;

class Die {
private:
	int value;
	bool played;
	std::shared_ptr<Player> owner;

public:
	Die(int val = 0, bool playedState = false) 
		: value(val)
		, played(playedState)
		, owner(nullptr) {}
		
	// Rule of five - explicitly default these
	Die(const Die&) = default;
	Die& operator=(const Die&) = default;
	Die(Die&&) noexcept = default;
	Die& operator=(Die&&) noexcept = default;
	~Die() = default;

	//randomized roll, changes single die value value
	int rollIt();
	bool isPlayed() const { return played; }
	void setPlayed(bool isPlayed) { played = isPlayed; }
	int getValue() const { return value; }
	std::shared_ptr<Player> getOwner() const { return owner; }
	void setOwner(std::shared_ptr<Player> dieOwner) { owner = dieOwner; }
	void printDieAscii() const;
};

#endif // VEGAS_DIE_H