//board.cc
#include "Board.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <random>

int Board::placeDie(Die& playerDie) {
    playerDie.setPlayed(true);
    dice.push_back(playerDie);
    return dice.size();
}

void Board::placeMoney() {
    // Available banknote denominations according to Vegas rules:
    // 6 each of $10,000, $40,000, $50,000
    // 8 each of $20,000, $30,000  
    // 5 each of $60,000, $70,000, $80,000, $90,000
    std::vector<int> denominations = {
        10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000
    };
    
    std::random_device seed;
    std::mt19937 gen(seed());
    std::uniform_int_distribution<int> dist(0, denominations.size() - 1);
    
    money.push_back(denominations[dist(gen)]);
}

void Board::placeMoney(int amount) {
    money.push_back(amount);
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