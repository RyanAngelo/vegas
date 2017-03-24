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
void giveAllPlayersDice( vector<Player>& players, int numberOfDice) {
	vector<Player>::iterator pIt;
	for (pIt = players.begin(); pIt != players.end(); ++pIt) {
		pIt->grabDice(numberOfDice);
		cout << pIt->getPlayerName() 
			<< " has " << pIt->getAllDice().size()
			<< " dice in hand " << endl;
	}
}

//Give the boards money
void giveBoardsMoney( vector<Board>& boards) {
	vector<Board>::iterator bIt;
	for ( bIt = boards.begin(); bIt != boards.end(); ++bIt) {
		while(bIt->getTotalMoney()<50000){
			bIt->placeMoney();
		}
		cout << "Money on Board #"<<bIt->getBoardId(); bIt->printMoney();
		cout << endl;
	}
}

//Get the number of players that are going to be playing from the user
int getNumberOfPlayers(){
	int numberOfPlayers;
	cout << "How many players?: ";
	cin >> numberOfPlayers;
	if(numberOfPlayers>6){
		cout << "The valid number of players is between 2 and 6" << endl;
		getNumberOfPlayers();
	}
	return numberOfPlayers;
}

bool someoneHasDice(const vector<Board>& boards, const vector<Player>& players, int numDicePerPlayer){
	unsigned int numDiceOnBoards = 0;
	vector<Board>::const_iterator bIt;
	for (bIt = boards.begin(); bIt != boards.end(); ++bIt) {
		numDiceOnBoards = numDiceOnBoards+bIt->totalDice();
	}
	cout << "Total Number of Dice on the Boards "<<numDiceOnBoards <<endl;
	cout << "Total Number of Players in Game: "<<players.size() <<endl;
	if(numDiceOnBoards == (players.size() * numDicePerPlayer) ){
		return false;
	}
	else{
		return true;
	}
}

vector<Player> createPlayers(int numberOfPlayers){
	vector<Player> activePlayers;
	for(int pCount=1; pCount<=numberOfPlayers; pCount++){
		string playerName;
		cout << "Player " << pCount << " please enter your name: ";
		cin >> playerName;
		cout << "Welcome, " << playerName << endl;
		activePlayers.push_back(Player(playerName,pCount));
	}
	return activePlayers;
}

vector<Board> createBoards(){
	vector<Board> activeBoards;
	for(int bCount=1; bCount<=6; bCount++){
		activeBoards.push_back(Board(bCount));
	}
	return activeBoards;
}

void printDicePerPlayer(map<class Player*, int> playerMappings){
	map<class Player*, int>::iterator mapIt;
	for ( mapIt = playerMappings.begin(); mapIt != playerMappings.end(); mapIt++ )
	{
	    cout << "\t" << mapIt->first->getPlayerName()  // string (key)
	              << ':'
	              << mapIt->second   // string's value 
	              << std::endl ;
	}
}


//Print the player status (total money)
void getPlayerStatus(vector<Player>& players){
	vector<Player>::iterator pIt;
	for ( pIt = players.begin(); pIt != players.end(); pIt++ )
	{
	    cout << "\t" << pIt->getPlayerName()
	              << ':'
	              << pIt->getTotalMoney()
	              << std::endl;
	}
}

//Get the number of dice per player on a board
map<class Player*, int> getDicePerPlayer(Board& board){
	vector<Die> dice = board.getDice();
	map<class Player*, int> playerMappings;
	vector<Die>::iterator dIt;
	for (dIt = dice.begin(); dIt != dice.end(); ++dIt) {
		Die& playerDie = *dIt;
		if( playerMappings.count(playerDie.getOwner()) ){
			playerMappings[playerDie.getOwner()]=playerMappings[playerDie.getOwner()]+1;
		}
		else{
			playerMappings[playerDie.getOwner()]=1;
		}
	}
	return playerMappings;
}

void getBoardStatus(vector<Board>& boards){
	cout << "----Board Status----" << endl;
	vector<Board>::iterator bIt;
	for (bIt = boards.begin(); bIt != boards.end(); ++bIt) {
		cout << "Board "<<bIt->getBoardId()<< ": "
			<<"\n\tTotal Dice: "<<bIt->totalDice() << endl;
		getDicePerPlayer(*bIt);
		vector<int>::iterator mIt;
		vector<int> money = bIt->getMoney();
		cout << "\tMoney: ";
		for ( mIt = money.begin(); mIt != money.end(); ++mIt) {
			cout<<*mIt<<" ";
		}
		cout << endl;
	}
}

/* Given the current players on the board that haven't tied or been paid out
*  get the next player that should receive a payout from the board
*/
class Player* findPlayerWithMostDice(map<class Player*, int>& playerMappings){
	typedef map<class Player*, int>::iterator iter;
	
	iter it = playerMappings.begin();
	iter end = playerMappings.end();
	
	int max_value = it->second;
	class Player* maxPlayer = it->first;
	
	bool duplicateFound=false;
	for( it = playerMappings.begin(); it != playerMappings.end(); ) {
	    if(it->second == max_value && it->first != maxPlayer) {
			cout << "Duplicate dice counts were found!"<<endl;
			duplicateFound=true;
	        it=playerMappings.erase(it);
	    }
		else if(it->second > max_value && it->first != maxPlayer){
			max_value=it->second;
			maxPlayer=it->first;
		}
		else{
			++it;
		}
	}
	if(duplicateFound){
		playerMappings.erase(maxPlayer);
		if(playerMappings.size()>0){	
			findPlayerWithMostDice(playerMappings);
		}
		else{
			return NULL;
		}
	}
	return maxPlayer;
}

//TODO: Currently causing segfault when called...
void distributeMoney(vector<Board>& boards){
	cout << "Distributing payout for "<<boards.size()<<" boards"<<endl;
	vector<Board>::iterator boardIterator;	
	for (boardIterator = boards.begin(); boardIterator != boards.end(); ++boardIterator) {
		cout << "Distributing money for Board "<<boardIterator->getBoardId()<<endl;
		map<class Player*, int> playerMappings = getDicePerPlayer(*boardIterator);
		while(boardIterator->getTotalMoney()>0 && playerMappings.size()>0){
			class Player* playerToGetMoney = findPlayerWithMostDice(playerMappings);
			//It's possible that no one will get any money on the board
			if(playerToGetMoney==NULL){
				cout<<"It's all tied up. No one gets any money for board "
					<<boardIterator->getBoardId()<<endl;
				continue;
			}
			//This person already got a payout. Remove them.
			playerMappings.erase(playerToGetMoney);
			cout<<"Player "<<playerToGetMoney->getPlayerName()<<" is getting a payout!";
			cout<<endl;
			playerToGetMoney->takeMoney(boardIterator->takeLargestBill());
		}
	}
}

int main() {
	int numDicePerPlayer=1;
	int numberOfRounds=2;
	
	int numberOfPlayers=getNumberOfPlayers();
	vector<Player> players=createPlayers(numberOfPlayers);
	vector<Board> boards=createBoards();
	
	for( int round=0; round<numberOfRounds; ++round ){
		giveAllPlayersDice(players, numDicePerPlayer);
		giveBoardsMoney(boards);
		while(someoneHasDice(boards, players, numDicePerPlayer)==true){
			string playerAnswer="n";
			vector<Player>::iterator pIt;
			for (pIt = players.begin(); pIt != players.end(); ++pIt) {
				if(pIt->hasActiveDice()==false){
					cout << pIt->getPlayerName() << ", you have no dice left. Turn passes!" << endl;
					continue;
				}
				while(playerAnswer != "yes" && playerAnswer != "y"){
					cout << pIt->getPlayerName() << ", ready to roll? (y or n)";
					cin >> playerAnswer;
					cout << endl;
				}
				//A positive answer to roll dice has been received
				pIt->rollAllDice();
				cout << pIt->getPlayerName() << ", which dice group would you like to place? ";
				cin >> playerAnswer;
				int intPlayerAnswer = atoi(playerAnswer.c_str());
				vector<Die> playerDice = pIt->getAllDice();
				pIt->addDiceToBoard(boards, intPlayerAnswer);
				getBoardStatus(boards); //get board status after placing dice
			}
		}
		distributeMoney(boards);
		vector<Player>::iterator pIt;
		for (pIt = players.begin(); pIt != players.end(); ++pIt) {
			cout << pIt->getPlayerName()<<" : "<<pIt->getTotalMoney()<<endl;
			//Cleanup player dice
			pIt->cleanupRound();
		}
		//Cleanup Board state
		vector<Board>::iterator bIt;
		for ( bIt = boards.begin(); bIt != boards.end(); ++bIt) {
			bIt->cleanupRound();
		}
	}
	cout << "Game over!" << endl;
	getPlayerStatus(players);
	players.clear();
	boards.clear();
	return 0;	
}