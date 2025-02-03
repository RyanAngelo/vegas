//Die source file
#include "Die.h"
#include <iostream>

using std::string;
using std::cout;

int Die::rollIt() {
	std::random_device seed;
	std::mt19937 gen(seed());
	std::uniform_int_distribution<int> dist(1, 6);
	value = dist(gen);
	return value;
}

void Die::printDieAscii() const {
	std::cout << value;
}