/*
FiveCardDraw.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/


#include "stdafx.h"
#include "FiveCardDraw.h"
#include "stdlib.h"

//#define cout std::cout //cout is not ambiguous

using namespace std;

//A default constructor for fiveCardDraw that initializes dealer to be the first person and discard to be empty. 
FiveCardDraw::FiveCardDraw() {
	dealer = 0;
	pot = 0;
	ante = 1;
	bet = 0;

	discardDeck = Deck();
	deck = Deck();
	deck.standardized(); //52 cards
}

//A public virtual before_turn method that asks the user cards to discard and then move them to discarDeck.
int FiveCardDraw::before_turn(Player& p) {
	//skip if the player has already folded
	if (p.isFold) return 0;

	cout << endl;
	cout << "Player " << p.name << " has " << p.hand << endl;
	cout << "Card to discard? Enter the indices separated by space in a line. " << endl;

	vector<bool> ifDelete;
	string toDiscard; //user response

	//remove card correspondingly
	if (p.isAuto) {
		ifDelete = p.hand.discardIndex();
		for (int k = 0; k < 5; k++) {
			if (ifDelete[k]) {
				cout << k + 1 << " ";
			}
		}
	} else { //wait for user input
		ifDelete = { false,false,false,false,false };
		while(toDiscard.length()==0) getline(cin, toDiscard); 
		toDiscard = " " + toDiscard + " ";
		for (int k = 1; k < 6; k++) {
			if (toDiscard.find(" "+to_string(k)+" ")!=string::npos) {
				ifDelete[k - 1] = true;
			}
		}	
	}

	//remove the card from the player to the discard desk
	for (int i = 4; i >= 0; i--) {
		if (ifDelete[i]) {
			discardDeck.addCard(p.hand[i]);
			p.hand.removeCard(i);
		}
	}

	return 0;
}

//A public virtual turn method that deals as many as that they have discarded.
int FiveCardDraw::turn(Player& p) {
	for (size_t i = p.hand.size(); i <5; i++) {
		if (deck.size() == 0) {
			if (discardDeck.size() == 0) throw NO_CARD_TO_DEAL; //when both decks are empty
			discardDeck.shuffle();
			p.hand << discardDeck; //deal from the discarded when no card in the main deck
		}
		else {
			p.hand << deck;
		}
	}
	return 0;
}

//A public virtual after_turn method that prints out the player's name and the contents of their hand.
int FiveCardDraw::after_turn(Player& p) {
	cout << p << endl;
	return 0;
}

//A public virtual before_round method that shuffles and then deals one card at a time from the main deck.
int FiveCardDraw::before_round() {
	deck.shuffle();
	int len = players.size();

	//players draw cards
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < len; j++) {
			int index = (dealer + j + 1) % len;
			if(players[index]->hand.size() < 5) players[index]->hand << deck;
		}
	}

	// each player place an ante of one chip to the pot
	for (int i = 0; i < len; i++) {
		pot += payChips(*players[i], ante);	
	}

	cout << endl;
	return 0;
}

//A public virtual round method that iterates through the players calling their turn method and then their after_turn method.
int FiveCardDraw::round() {
	int len = players.size();

	//players decide whether to bet some chips or not - first phase
	bet_in_turn();

	//change some cards in hand
	if (countActive() > 1) {
		//players discard some of the five cards
		for (int i = 0; i <len; i++) before_turn(*players[i]);

		//deal cards until each player has five cards
		for (int i = 0; i < len; i++) turn(*players[i]);

		//players decide whether to bet some chips or not - second phase
		bet_in_turn();
	}

	//print player ranks before sorting
	cout << endl;
	for (int i = 0; i < len; i++) {
		after_turn(*players[i]);
	}

	return 0;
}

//A public virtual after_round method that sorts players by hand rank, recycles all cards, and asks whether to leave or join.
int FiveCardDraw::after_round() {
	int len = players.size();

	//sort a temporary vector of pointers to players (a copy of the vector member variable)
	vector<shared_ptr<Player>> tempPlayers;
	for (int i = 0; i < len; i++) {
		tempPlayers.push_back(players[i]);
	}
	sort(tempPlayers.begin(), tempPlayers.end(), handRankCompare);	

	//print out player ranks after sorting
	cout << endl;
	for (int i = 0; i < len; i++) {
		after_turn(*tempPlayers[i]);
	}

	//find winner's combo
	int maxIndex = -1;
	for (int i = len - 1; i >= 0; i--) {
		if (tempPlayers[i]->isFold == false){
			maxIndex = i;
			break;
		}
	}
	if (maxIndex == -1) throw NO_ACTIVES;
	int maxHash = tempPlayers[maxIndex]->hand.hashHand();
	//HandRank maxRank = static_cast<HandRank>(temp); //cast from int to enum

	//calculate wins and losses
	vector<shared_ptr<Player>> winners;
	for (int i = len - 1; i >= 0; i--) {
		if ((players[i]->isFold == false) && (players[i]->hand.hashHand() == maxHash)) {
			++players[i]->won;
			players[i]->chip += players[i]->bet;
			winners.push_back(players[i]);
		}
		else {
			++players[i]->lost;
			pot += players[i]->bet;
		}

		//reset variables for each player
		players[i]->bet = 0; 
		players[i]->isFold = false;

		//move cards from players to the main deck
		for (int j = players[i]->hand.size()-1; j >=0; j--) {
			deck.addCard(players[i]->hand[j]);
			players[i]->hand.removeCard(j);
		}
	}

	//calculate number of winners
	int numOfWinners = winners.size();
	if (numOfWinners == 0) throw NO_WINNERS;
	cout << endl;
	cout << numOfWinners << " winner(s) sharing " << pot << " chips: " << endl;

	//distribute chips from the pot to winner(s)
	int part = (int) floor(pot/numOfWinners);
	for (int i = 0; i < numOfWinners; i++) {
		winners[i]->chip += part;
		cout << winners[i]->name << endl;
	}

	//reset variables for the game
	pot = 0;
	bet = 0;
	
	//move all cards from discardDeck to the main deck
	while (discardDeck.size() > 0) {
		deck.addCard(discardDeck.popCard());
	}

	//some auto players leave the game
	autoPlayerLeave();

	//ask players who have zero chips
	for (int i = 0; i < len; i++) {
		if (players[i]->chip == 0) {
			string quitName = players[i]->name;
			//re-add the player
			players.erase(players.begin() + i);
			addPlayer(quitName); //where zero-chip condition is checked
		}

	}

	//ask the rest of the players whether to leave the game
	string checktemp;
	string quitName;
	ofstream output;
	int quitIndex =-1;
	bool findNo;
	do {

	    cout << endl;
		cout << "Any player leaving? Enter 'No' for nobody. " << endl;
		//assume that no player is named 'no'
		cout << "Player's name: " << endl;
		cin >> quitName;
		checktemp = quitName;
		transform(quitName.begin(), quitName.end(), checktemp.begin(), ::tolower); //accept 'NO' 'no' 'No' 'nO'
		if (checktemp.find("no") != string::npos && checktemp.length()==2) {
			findNo = true;
		}
		else if (checktemp.find("no*") != string::npos && checktemp.length() == 3) { //check for "no* "
			findNo = true;
		}else{
			findNo = false; 
		}
		if (!findNo) {
			int len = players.size();
			for (int i = 0; i < len; i++) {
				if (quitName == (*players[i]).name) {
					quitIndex = i;
				}
			}

			if (quitIndex != -1) {
				players.erase(players.begin() + quitIndex);
			}
			else {
				cout << "The player " << quitName << " is not currently playing!" << endl;
			}
		}
		
	} while (!findNo);

	//ask whether to join the game
	string joinName;
	
	do {
		cout << endl;
		cout << "Any player joining? Enter 'No' for nobody. " << endl;
		cout << "Player's name: " << endl;
		cin >> joinName;
		checktemp = joinName;
		transform(joinName.begin(), joinName.end(), checktemp.begin(), ::tolower); //accept 'NO' 'no' 'No' 'nO'
		if (checktemp.find("no") != string::npos && joinName.length() == 2) {
			findNo = true;
		}
		else if (checktemp.find("no*") != string::npos && checktemp.length() == 3) { //check for "no* "
			findNo = true;
		}else { 
			findNo = false; 
		}
		if(!findNo) addPlayer(joinName); //it checks for duplicates
	} while (!findNo);

	cout << endl;

	//Switch to next dealer
	if(players.size()!=0) dealer = (dealer + 1) % players.size(); 

	return 0;
}

int FiveCardDraw::bet_in_turn() {
	int len = players.size();
	int active;

	//loop until one player remains on bet
	do {
		for (int i = 0; i < len; i++) {
			pot += betChips(*players[i]);
			active = countActive();
			if (active <= 1) break;
		}
	} while (active > 1);

	return 0;

}

// Claculate numer of chips a player pays.
unsigned int FiveCardDraw::payChips(Player& p, unsigned int amount) {
	int payed = amount;
	if (p.chip > amount) { //add the amount equal to ante
		p.chip -= amount;
	}
	else { //add all the chips
		payed = p.chip;
		p.chip = 0;
	}
	return payed;
}

// Ask and deduct chips from player and add to a temporary bet variable
unsigned int FiveCardDraw::betChips(Player& p) {
	//skip if the player has already folded
	if (p.isFold) return 0;

	//range for the number of chips valid to bet
	int min = bet;
	int max;

	cout << endl;
	cout << p << endl;

    //ask player if all-in
	if (bet >= p.chip) { 
		cout << "Please enter '"<<p.chip<<"' for ALL-IN, or 'f' for FOLD: " << endl;
		max = p.chip;
    //no one has bet yet
	} else if (bet == 0) {
		max = 2;
		cout << "Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD: " << endl;
	}
    //someone has bet already
	else if (bet + 1 >= p.chip) {
		max = bet + 1;
		cout << "Please enter "<<bet<<" for CALL, '"<<bet+1<<" for RAISE, or 'f' for FOLD: " << endl;
	} else {
		max = bet + 2;
		cout << "Please enter " << bet << " for CALL, '" << bet + 1 << "' or '" << bet + 2 << "' for RAISE, or 'f' for FOLD: " << endl;
	}
	
	string str;
	int num = -1;

	//get the number to bet from user input
	do {
		getline(cin, str);
		bool findNo = (str.find("f") != string::npos) && (str.length() == 1);
		if (findNo) {
			num = 0;
			p.isFold = true;
			break;
		}
		else if (str.length() != 0) {
			num = atoi(str.c_str());
		}
	} while (num < min || num > max);
	
	unsigned int numToBet = (unsigned int) num;
	if (numToBet > bet) bet = numToBet;
	return numToBet;
}

//Check for autoplayers whether each of them leaves the game.
int FiveCardDraw::autoPlayerLeave() {
	vector<int> autoPlayers = findAuto();
	int numAuto = autoPlayers.size();
	int numPlayers = players.size();
	unsigned int leaveNum;
	string name;

	//immediate update associated files
	ofstream output;
	for (int i = 0; i < numPlayers; i++) {
		name = (*players[i]).name;
		if (find(autoPlayers.begin(), autoPlayers.end(), i) != autoPlayers.end()) {
			name = name.substr(0, name.size() - 1);
		}
		output.close();
		output.open(name+".txt");
		output << *players[i];
	}

	//auto players leave with probablity
	for (int i = numAuto-1; i >=0 ;i--) {
		leaveNum = rand() % 100;
		if (autoPlayers[i] == 0) { //the last place
			if (leaveNum < 90) {
				players.erase(players.begin() + autoPlayers[i]);
			}
		}
		else if (autoPlayers[i]==numPlayers) { //the first place
			if (leaveNum < 10) {
				players.erase(players.begin() + autoPlayers[i]);
			}
		}
		else {
			if (leaveNum < 50) { //otherwise
				players.erase(players.begin() + autoPlayers[i]);
			}
		}

	}

	return 0;
}

//Return a list of all auto players.
vector<int> FiveCardDraw::findAuto() {
	int currentPlayerNum = players.size();
	vector<int> autoPlayers;
	for (int i = 0; i < currentPlayerNum; i++) {
		char last = (*players[i]).name.back();
		if (last == '*') {
			autoPlayers.push_back(i);
		}
	}
	return autoPlayers;
}

//Count number of active players who have not folded.
int FiveCardDraw::countActive() {
	int numPlayers = players.size();
	int active = 0;
	for (int i = 0; i < numPlayers; i++) {
		if (players[i]->isFold == false) active++;
	}
	return active;
}
