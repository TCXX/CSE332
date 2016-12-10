/*
PokerGame.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/


#include "stdafx.h"
#include "PokerGame.h"
#include "stdlib.h"

using namespace std;

//A default constructor that initializes dealer to be the first person and discard to be empty. 
PokerGame::PokerGame() {
	dealer = 0;
	pot = 0;
	ante = 1;
	bet = 0;

	discardDeck = Deck();
	deck = Deck();
	deck.standardized(); //52 cards
}

//Discard cards for a player and move them to discarDeck.
int PokerGame::discardCards(Player& p) {
	//skip if the player has already folded
	if (p.isFold) return 0;

	cout << endl;
	cout << p.toString(OWNER) << endl;
	cout << "Card to discard? Enter the indices separated by space in a line. " << endl;

	vector<bool> ifDelete;
	string toDiscard; //user response

	if (p.isAuto) { //discard cards for an autoplayer
		ifDelete = p.hand.discardIndex();
		for (size_t k = 0; k < MAX_CARDS_IN_HAND; k++) {
			if (ifDelete[k]) {
				cout << k + 1 << " ";
			}
		}
	}
	else { //wait for user input
		while (toDiscard.length() == 0) getline(cin, toDiscard);
		toDiscard = " " + toDiscard + " ";
		for (size_t k = 0; k < MAX_CARDS_IN_HAND; k++) {
			if (toDiscard.find(" " + to_string(k+1) + " ") != string::npos) {
				ifDelete.push_back(true);
			}
			else ifDelete.push_back(false);
		}
	}

	//remove the card to discard desk
	for (size_t i = MAX_CARDS_IN_HAND; i > 0; i--) {
		if (ifDelete[i-1]) {
			discardDeck.addCard(p.hand[i-1]);
			p.hand.removeCard(i-1);
		}
	}

	return 0;
}

//Deal cards to given player until hand is full.
int PokerGame::dealUntilFull(Player& p, Visibility vis) {
	for (size_t i = p.hand.size(); i < MAX_CARDS_IN_HAND; i++) {
		dealCard(p, vis);
	}
	return 0;
}

//Deal a card to each player of the specified type.
int PokerGame::dealAround(Visibility vis) {
	size_t len = players.size();
	for (size_t j = 0; j < len; j++) {
		size_t index = (dealer + j + 1) % len;
		if (players[index]->hand.size() < MAX_CARDS_IN_HAND) {
			dealCard(*players[index], vis);
		}
	}
	return 0;
}

//Deal a card to the given player of the given type.
int PokerGame::dealCard(Player& p, Visibility vis) {
	Card c;
	if (deck.size() == 0) {
		if (discardDeck.size() == 0) throw NO_CARD_TO_DEAL; //when both decks are empty
		discardDeck.shuffle();
		c = discardDeck.popCard(); //deal from the discarded when no card in the main deck
	}
	else {
		c = deck.popCard();
	}

	c.visible = vis;
	p.hand.pushCard(c);

	return 0;
}

////Deal a same card to all players of the given type.
int PokerGame::dealSameToAll(Visibility vis) {
	Card c;
	if (deck.size() == 0) {
		if (discardDeck.size() == 0) throw NO_CARD_TO_DEAL; //when both decks are empty
		discardDeck.shuffle();
		c = discardDeck.popCard(); //deal from the discarded when no card in the main deck
	}
	else {
		c = deck.popCard();
	}

	c.visible = vis;
	size_t len = players.size();
	for (size_t i = 0; i < len; i++) players[i]->hand.pushCard(c);

	return 0;
}

//Check chips and pay ante.
int PokerGame::before_round() {
	deck.shuffle();
	CheckChips();

	// each player place an ante to the pot
	size_t len = players.size();
	for (size_t i = 0; i < len; i++) {
		pot += payChips(*players[i], ante);
	}

	return 0;
}

//round() will be implemented in subclasses

//Sorts player by hand rank, ask whether to leave or join, and save player data.
int PokerGame::after_round() {
	size_t len = players.size();

	//sort a temporary vector of pointers to players
	vector<shared_ptr<Player>> tempPlayers;
	for (size_t i = 0; i < len; i++) {
		tempPlayers.push_back(players[i]);
	}
	sort(tempPlayers.begin(), tempPlayers.end(), playerRank); 

	//print out player ranks after sorting
	cout << endl;
	for (size_t i = 0; i < len; i++) {
		cout << tempPlayers[i]->toString(ADMIN) << endl;
	}

	//find winner's combo
	size_t maxIndex = len; //invalid value to indicate undefined
	for (size_t i = 0; i < len; i++) {
		if (tempPlayers[i]->isFold == false) {
			maxIndex = i;
			break;
		}
	}
	if (maxIndex == len) throw NO_ACTIVES;
	

	//calculate wins and losses
	vector<shared_ptr<Player>> winners;
	if(countActive()>1){
		int maxHash = tempPlayers[maxIndex]->hand.findMaxHash();
		for (size_t i = len; i > 0; i--) {
			if ((players[i - 1]->isFold == false) && (players[i - 1]->hand.findMaxHash() == maxHash)) {
				++players[i - 1]->won;
				//players[i-1]->chip += players[i-1]->bet;
				winners.push_back(players[i - 1]);
			}
			else {
				++players[i - 1]->lost;
				//pot += players[i-1]->bet;
			}

			//reset variables for each player
			players[i - 1]->bet = 0;
			players[i - 1]->hand = Hand();
			players[i - 1]->isFold = false;
		}
	}
	else {
		winners.push_back(tempPlayers[maxIndex]);
	}
	//calculate number of winners
	size_t numOfWinners = winners.size();
	if (numOfWinners == 0) throw NO_WINNERS;
	cout << endl;
	cout << numOfWinners << " winner(s) sharing " << pot << " chips: " << endl;

	//distribute chips from the pot to winner(s)
	int part = (int)floor(pot / numOfWinners);
	for (size_t i = 0; i < numOfWinners; i++) {
		winners[i]->chip += part;
		cout << winners[i]->name << endl;
	}

	//reset variables for the game
	pot = 0;
	bet = 0;
	deck = Deck();
	discardDeck = Deck();
	deck.standardized(); //52 cards

	//ask players who have zero chips to reset
	CheckChips();
	len = players.size();

	//save the game
	saveToFile();

	//some auto players leave the game
	autoPlayerLeave();

	//ask the rest of the players whether to leave the game
	string checktemp;
	string quitName;
	ofstream output;
	int quitIndex = -1;
	bool findNo;
	do {

		cout << endl;
		cout << "Any player leaving? Enter 'No' for nobody. " << endl;
		//assume that no player is named 'no'
		cout << "Player's name: " << endl;
		cin >> quitName;
		checktemp = quitName;
		transform(quitName.begin(), quitName.end(), checktemp.begin(), ::tolower); //accept 'NO' 'no' 'No' 'nO'
		if (checktemp.find("no") != string::npos && checktemp.length() == 2) {
			findNo = true;
		}
		else if (checktemp.find("no*") != string::npos && checktemp.length() == 3) { //check for "no* "
			findNo = true;
		}
		else {
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

		quitIndex = -1;
		
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
		}
		else {
			findNo = false;
		}
		if (!findNo) addPlayer(joinName); //it checks for duplicates
	} while (!findNo);

	cout << endl;

	//Switch to next dealer
	if (players.size() != 0) dealer = (dealer + 1) % players.size();

	return 0;
}

//Print out a list of current players under given mode.
int Game::printPlayers(AccessMode mode) {
	size_t len = players.size();
	cout << endl;
	for (size_t i = 0; i < len; i++) {
		cout << players[i]->toString(mode) << endl;
	}
	return 0;
}

//All players bet until done.
int PokerGame::bet_in_turn() {
	size_t len = players.size();
	if (len == 0) throw NO_PLAYERS;

	int active; // the current number of unfolded players
	size_t raiser = 0; // the last player who raises the bet; used to determine if betting phase ends
	size_t i = 0; // the current player to ask for response
	bool finish = false; // whether the betting phase should be terminated

	do {
		unsigned int before = bet;
		pot+=betChips(*players[i]);
		unsigned int after = bet;
		if (before!=after) { //someone raised
			raiser = i;
		}

		active = countActive();
		if (active <= 1) finish = true;
		i = (i + 1) % len;
		if (i == raiser) finish = true;

	} while (!finish); // two breaks

	cout << endl;

	return 0;
}

// Calculate numer of chips a player pays.
unsigned int PokerGame::payChips(Player& p, unsigned int amount) {
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

// Ask and deduct chips and add to player's bet variable
unsigned int PokerGame::betChips(Player& p) {
	//skip if the player has already folded
	if (p.isFold) return 0;

	//check pot's capacity
	if (pot > 300) throw POT_OVERFLOW;

	//range for the number of chips valid to bet
	int min;
	int max;

	//whether player's bet can affect current bet
	bool changeBet = true;

	cout << endl;
	cout << p.toString(OWNER) << "bet " << bet << endl;

	//skip player without chips
	if (p.chip == 0) {
		cout << "Run out of chips! " << endl;
		return 0;
	}

	if (bet == 0) {
		min = 0;
		if (p.chip >= 2) {
			max = 2;
			cout << "Please enter '0' for CHECK, '1' or '2' for BET, or 'f' for FOLD: " << endl;
		}
		else {
			max = p.chip;
			cout << "Please enter '0' for CHECK, '1' for BET, or 'f' for FOLD: " << endl;
		}
		
	}
	//ask player if all-in
	else if (bet >= p.chip+p.bet) {
		min = p.chip + p.bet;
		max = min;
		changeBet = false;
		cout << "Please enter '" << p.chip + p.bet << "' for ALL-IN, or 'f' for FOLD: " << endl;
	}
	//someone has bet already
	else if (bet - p.bet + 1 >= p.chip) {
		min = bet;
		max = bet + 1;
		cout << "Please enter " << bet << " for CALL, '" << bet + 1 << " for RAISE, or 'f' for FOLD: " << endl;
	}
	else {
		min = bet;
		max = bet + 2;
		cout << "Please enter " << bet << " for CALL, '" << bet + 1 << "' or '" << bet + 2 << "' for RAISE, or 'f' for FOLD: " << endl;
	}

	string str;
	int num = -1;
	unsigned int numToBet;
	unsigned int due;

	if (p.isAuto) { //bet for autoplayer
		numToBet = autoPlayerBet(max, min);
		cout << numToBet << " (auto) " << endl;
	}
	else { //get the number to bet from user input
		do {
			getline(cin, str);
			bool findNo = (str.find("f") != string::npos) && (str.length() == 1);
			if (findNo) { //player chooses to fold
				num = bet;
				changeBet = false;
				size_t len = p.hand.size();
				for (size_t i = 0; i < len; i++) p.hand[i].visible = NEVER_SEEN;
				p.isFold = true;
				break;
			}
			else if (str.length() != 0) {
				num = atoi(str.c_str());
			}
		} while (num < min || num > max);
		numToBet = (unsigned int)num;
	}

	if (numToBet > p.bet) {
		due = numToBet - p.bet; //extra chips that needs to pay
	}
	else { //all in
		due = numToBet; 
		changeBet = false;
	}

	p.bet += payChips(p, due);
	if (changeBet) bet = numToBet;
	
	return due;
}

//Decide for autoplayers what to bet.
unsigned int PokerGame::autoPlayerBet(int max, int min) {
	int leaveNum = ((int) rand() % (max-min)) + min;
	return (unsigned int)leaveNum;
}

//Check if every player has chips to bet.
void PokerGame::CheckChips() {
	size_t len = players.size();
	for (size_t i = len; i > 0; i--) { //not mess up with order
		if (players[i - 1]->chip == 0) {
			string ans;
			cout << "Player " << players[i - 1]->name << " has no chips. Reset? (yes/no) " << endl;
			do {
				getline(cin, ans);
				transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
			} while (ans != "yes" && ans != "no");
			if (ans == "no") {
				players.erase(players.begin() + i - 1);
			}
			else {
				players[i - 1]->reset();
			}
		}
	}
}
