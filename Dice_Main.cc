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
	printDicePerPlayer(playerMappings);
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

void getMoneyOutput(vector<Board>& boards){
	vector<Board>::iterator bIt;
	for (bIt = boards.begin(); bIt != boards.end(); ++bIt) {
		bIt->placeMoney();
	}
}


//TODO: Need a way to continue looping until no dups are found
class Player* findPlayerWithMostDice(map<class Player*, int> playerMappings){
	typedef map<class Player*, int>::iterator iter;
	
	iter it = playerMappings.begin();
	iter end = playerMappings.end();
	
	int max_value = it->second;
	class Player* maxPlayer = it->first;
	
	bool duplicateFound=false;
	for( it = playerMappings.begin(); it != end; ) {
	    if(it->second == max_value && it->first != maxPlayer) {
			duplicateFound=true;
	        it=playerMappings.erase(it);
			if(playerMappings.size()==0){
				//There are no winners for this set :-(
				return nullptr;
			}
	    }
		else{
			++it;
		}
	}
	if(duplicateFound){
		findPlayerWithMostDice(playerMappings);
	}
	return maxPlayer;
}

//TODO: Currently causing segfault when called...
void distributeMoney(vector<Board>& boards){
	cout << "Distributing payout for "<<boards.size()<<" boards"<<endl;
	vector<Board>::iterator bIt;
	for (bIt = boards.begin(); bIt != boards.end(); ++bIt) {
		//TODO: Need more checks here for distributing money, this just gives all the money
		//To the leading player...
		cout << "Distributing money for Board "<< bIt -> getBoardId();
		while(bIt->getTotalMoney()>0){ //It's possible that some money will go to nobody.
			map<class Player*, int> playerMappings = getDicePerPlayer(*bIt);
			class Player* playerToGetMoney = findPlayerWithMostDice(playerMappings);
			cout<<"Player "<<playerToGetMoney->getPlayerName()<<" is getting a payout!";
			//playerToGetMoney->takeMoney(bIt->takeLargestBill());
		}
			
	}
	//std::cout << *std::max_element(std::begin(list), std::end(list));
}

int main() {
	int numberOfPlayers=getNumberOfPlayers();
	int numDicePerPlayer=1;
	vector<Player> players=createPlayers(numberOfPlayers);
	vector<Board> boards=createBoards();
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
			getBoardStatus(boards); //get the board status after the roll and die placement
		}
	}
	distributeMoney(boards);
	cout << "Game over!" << endl;
	//getBoardStatus(boards);
	vector<Player>::iterator pIt;
	for (pIt = players.begin(); pIt != players.end(); ++pIt) {
		cout << pIt->getPlayerName()<<" : "<<pIt->getTotalMoney()<<endl;
	}
	return 0;	
}