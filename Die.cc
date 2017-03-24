//Die source file
#include "Die.h"

using std::string;
using std::cout;

int Die::rollIt(){
	std::random_device seed;
	std::mt19937 gen(seed());
	std::uniform_int_distribution<int> dist(1, 6);
	value=dist(gen);
	return value;
}

bool Die::isPlayed(){
	return played;
}

void Die::setPlayed(bool isPlayed){
	played = isPlayed;
}

int Die::getValue() const{
	return value;
}

class Player* Die::getOwner(){
	return owner;
}

void Die::setOwner(class Player* dieOwner){
	owner = dieOwner;
}

void Die::printDieAscii(){
	cout << value;
}