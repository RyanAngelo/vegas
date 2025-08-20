//Main for Dice Rolling game
//Roll die against up to 6 opponents and place on boards
//Las Vegas Style, to find out who wins the money under each board
//Whoever has the most dice on a board wins the money.
//Whoever has the most money after 4 rounds wins the game

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <memory>
#include <algorithm>
#include <stdexcept>

#include "Die.h"
#include "Player.h"
#include "Board.h"

using std::cout;
using std::vector;
using std::endl;
using std::cin;
using std::string;
using std::map;

//Give all players that are in the game dice to start out with.
void giveAllPlayersDice(std::vector<std::shared_ptr<Player>>& players, int numberOfDice) {
	for (auto& player : players) {
		player->grabDice(numberOfDice);
		cout << player->getPlayerName() 
			<< " has " << player->getAllDice().size()
			<< " dice in hand " << endl;
	}
}

//Give the boards money
void giveBoardsMoney(std::vector<Board>& boards) {
	for (auto& board : boards) {
		while(board.getTotalMoney()<50000){
			board.placeMoney();
		}
		cout << "Money on Board #"<<board.getBoardId(); board.printMoney();
		cout << endl;
	}
}

//Get the number of players that are going to be playing from the user
int getNumberOfPlayers() {
	int numberOfPlayers;
	while (true) {
		cout << "How many players? (2-6): ";
		if (cin >> numberOfPlayers && numberOfPlayers >= 2 && numberOfPlayers <= 6) {
			return numberOfPlayers;
		}
		cout << "Please enter a number between 2 and 6" << endl;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

bool someoneHasDice(const std::vector<Board>& boards, const std::vector<std::shared_ptr<Player>>& players, int numDicePerPlayer) {
	size_t numDiceOnBoards = std::accumulate(boards.begin(), boards.end(), 0,
		[](size_t sum, const Board& board) { return sum + board.totalDice(); });
		
	cout << "Total Number of Dice on the Boards "<<numDiceOnBoards <<endl;
	cout << "Total Number of Players in Game: "<<players.size() <<endl;
	if(numDiceOnBoards == (players.size() * numDicePerPlayer) ){
		return false;
	}
	else{
		return true;
	}
}

std::vector<std::shared_ptr<Player>> createPlayers(int numberOfPlayers) {
	std::vector<std::shared_ptr<Player>> activePlayers;
	activePlayers.reserve(numberOfPlayers);
	
	for(int pCount=1; pCount<=numberOfPlayers; pCount++){
		string playerName;
		cout << "Player " << pCount << " please enter your name: ";
		cin >> playerName;
		cout << "Welcome, " << playerName << endl;
		activePlayers.push_back(std::make_shared<Player>(playerName,pCount));
	}
	return activePlayers;
}

std::vector<Board> createBoards(){
	std::vector<Board> activeBoards;
	activeBoards.reserve(6);
	for(int bCount=1; bCount<=6; bCount++){
		activeBoards.emplace_back(bCount);
	}
	return activeBoards;
}

void printDicePerPlayer(const std::map<std::shared_ptr<Player>, int>& playerMappings) {
    for (const auto& [player, count] : playerMappings) {
        std::cout << "\t" << player->getPlayerName() 
                 << ": " << count << std::endl;
    }
}


//Print the player status (total money)
void getPlayerStatus(const std::vector<std::shared_ptr<Player>>& players){
	for (const auto& player : players) {
	    cout << "\t" << player->getPlayerName()
	              << ':'
	              << player->getTotalMoney()
	              << std::endl;
	}
}

//Get the number of dice per player on a board
std::map<std::shared_ptr<Player>, int> getDicePerPlayer(const Board& board) {
    const auto& dice = board.getDice();
    std::map<std::shared_ptr<Player>, int> playerMappings;
    
    for (const auto& die : dice) {
        auto owner = die.getOwner();
        playerMappings[owner]++;
    }
    return playerMappings;
}

void getBoardStatus(const std::vector<Board>& boards){
	cout << "----Board Status----" << endl;
	for (const auto& board : boards) {
		cout << "Board "<<board.getBoardId()<< ": "
			<<"\n\tTotal Dice: "<<board.totalDice() << endl;
		
		// Show dice ownership
		auto playerMappings = getDicePerPlayer(board);
		if (!playerMappings.empty()) {
			cout << "\tDice by player: ";
			for (const auto& [player, count] : playerMappings) {
				cout << player->getPlayerName() << "(" << count << ") ";
			}
			cout << endl;
		} else {
			cout << "\tNo dice placed yet" << endl;
		}
		
		vector<int>::iterator mIt;
		vector<int> money = board.getMoney();
		cout << "\tMoney: ";
		for ( mIt = money.begin(); mIt != money.end(); ++mIt) {
			cout<<*mIt<<" ";
		}
		cout << endl;
	}
}

/* Remove all players who have tied dice counts according to Vegas rules */
void removeTiedPlayers(std::map<std::shared_ptr<Player>, int>& playerMappings) {
    std::map<int, std::vector<std::shared_ptr<Player>>> countGroups;
    
    // Group players by dice count
    for (const auto& [player, count] : playerMappings) {
        countGroups[count].push_back(player);
    }
    
    // Remove all players in groups with more than one player (ties)
    for (const auto& [count, players] : countGroups) {
        if (players.size() > 1) {
            std::cout << "Tie detected with " << count << " dice: ";
            for (const auto& player : players) {
                std::cout << player->getPlayerName() << " ";
                playerMappings.erase(player);
            }
            std::cout << "- all tied players removed!" << std::endl;
        }
    }
}

/* Given the current players on the board that haven't tied or been paid out
*  get the next player that should receive a payout from the board
*/
std::shared_ptr<Player> findPlayerWithMostDice(std::map<std::shared_ptr<Player>, int>& playerMappings) {
    if (playerMappings.empty()) {
        return nullptr;
    }
    
    auto maxIt = std::max_element(playerMappings.begin(), playerMappings.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    
    auto maxPlayer = maxIt->first;
    playerMappings.erase(maxIt);
    return maxPlayer;
}

void distributeMoney(std::vector<Board>& boards, std::vector<std::shared_ptr<Player>>& players) {
    (void)players; // Suppress unused parameter warning
    std::cout << "Distributing payout for " << boards.size() << " boards" << std::endl;
    
    for (auto& board : boards) {
        std::cout << "Distributing money for Board " << board.getBoardId() << std::endl;
        auto playerMappings = getDicePerPlayer(board);
        
        // First, remove all tied players according to Vegas rules
        removeTiedPlayers(playerMappings);
        
        // Then distribute money to remaining players in order of dice count
        while (board.getTotalMoney() > 0 && !playerMappings.empty()) {
            auto winner = findPlayerWithMostDice(playerMappings);
            if (!winner) {
                break; // This shouldn't happen after removing ties
            }
            
            int amount = board.takeLargestBill();
            std::cout << "Player " << winner->getPlayerName() 
                     << " wins $" << amount << std::endl;
            winner->takeMoney(amount);
        }
    }
}

int main() {
	constexpr int numDicePerPlayer = 8;
	constexpr int numberOfRounds = 4;
	
	int numberOfPlayers = getNumberOfPlayers();
	auto players = createPlayers(numberOfPlayers);
	auto boards = createBoards();
	
	for( int round=0; round<numberOfRounds; ++round ){
		cout << "\nRound " << (round + 1) << " begins!\n" << endl;
		
		giveAllPlayersDice(players, numDicePerPlayer);
		giveBoardsMoney(boards);
		while(someoneHasDice(boards, players, numDicePerPlayer)==true){
			for (auto& player : players) {
				if(player->hasActiveDice()==false){
					cout << player->getPlayerName() << ", you have no dice left. Turn passes!" << endl;
					continue;
				}
				string playerAnswer;
				do {
					cout << player->getPlayerName() << ", ready to roll? (y/n): ";
					cin >> playerAnswer;
				} while(playerAnswer != "yes" && playerAnswer != "y");
				
				player->rollAllDice();
				
				// Show available dice values to choose from
				auto availableValues = player->getAvailableDiceValues();
				if (availableValues.empty()) {
					cout << player->getPlayerName() << " has no dice left!" << endl;
					continue;
				}
				
				cout << "Available dice values to place: ";
				for (int value : availableValues) {
					cout << value << " ";
				}
				cout << endl;
				
				int boardChoice;
				do {
					cout << player->getPlayerName() << ", choose which dice value to place (must be one you rolled): ";
					cin >> boardChoice;
				} while(std::find(availableValues.begin(), availableValues.end(), boardChoice) == availableValues.end());
				
				player->addDiceToBoard(boards, boardChoice);
				getBoardStatus(boards); //get board status after placing dice
			}
		}
		distributeMoney(boards, players);
		for (auto& player : players) {
			cout << player->getPlayerName()<<" : "<<player->getTotalMoney()<<endl;
			//Cleanup player dice
			player->cleanupRound();
		}
		//Cleanup Board state
		for (auto& board : boards) {
			board.cleanupRound();
		}
		
		cout << "\nEnd of round " << (round + 1) << " standings:" << endl;
		getPlayerStatus(players);
	}
	cout << "\nGame over! Final standings:" << endl;
	getPlayerStatus(players);
	players.clear();
	boards.clear();
	return 0;	
}