/*
PokerGame.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/


#include "stdafx.h"
#include "PokerGame.h"
#include "stdlib.h"

//#define cout std::cout //cout is not ambiguous

using namespace std;

//A default constructor for fiveCardDraw that initializes dealer to be the first person and discard to be empty. 
PokerGame::PokerGame() {
	dealer = 0;
	pot = 0;
	ante = 1;
	bet = 0;

	discardDeck = Deck();
	deck = Deck();
	deck.standardized(); //52 cards
}

//A method that asks the user cards to discard and then move them to discarDeck.
int PokerGame::discardCards(Player& p) {
	//skip if the player has already folded
	if (p.isFold) return 0;

	cout << endl;
	cout << p.toString(OWNER) << endl;
	cout << "Card to discard? Enter the indices separated by space in a line. " << endl;

	vector<bool> ifDelete;
	string toDiscard; //user response

	//remove card correspondingly
	if (p.isAuto) {
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

	//remove the card from the player to the discard desk
	for (size_t i = MAX_CARDS_IN_HAND; i > 0; i--) {
		if (ifDelete[i-1]) {
			discardDeck.addCard(p.hand[i-1]);
			p.hand.removeCard(i-1);
		}
	}

	return 0;
}

//A method that deals as many as that they have discarded.
int PokerGame::dealUntilFull(Player& p, Visibility vis) {
	for (size_t i = p.hand.size(); i < MAX_CARDS_IN_HAND; i++) {
		dealCard(p, vis);
	}
	return 0;
}

// deal a card to each of all players a card of the specified type.
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

//deal a card to a specified player of a specified type.
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

//
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

//A public virtual before_round method that shuffles and then deals one card at a time from the main deck.
int PokerGame::before_round() {
	deck.shuffle();
	CheckChips();

	// each player place an ante of one chip to the pot
	size_t len = players.size();
	for (size_t i = 0; i < len; i++) {
		pot += payChips(*players[i], ante);
	}

	return 0;
}

//round() will be implemented in subclasses

//A public virtual after_round method that sorts players by hand rank, recycles all cards, and asks whether to leave or join.
int PokerGame::after_round() {
	size_t len = players.size();

	//sort a temporary vector of pointers to players (a copy of the vector member variable)
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

	//ask players who have zero chips
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

int Game::printPlayers(AccessMode mode) {
	size_t len = players.size();
	cout << endl;
	for (size_t i = 0; i < len; i++) {
		cout << players[i]->toString(mode) << endl;
	}
	return 0;
}

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

// Claculate numer of chips a player pays.
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

// Ask and deduct chips from player and add to a temporary bet variable
unsigned int PokerGame::betChips(Player& p) {
	//skip if the player has already folded
	if (p.isFold) return 0;

	//range for the number of chips valid to bet
	int min;
	int max;

	//whether player's bet can affect current bet
	bool changeBet = true;

	cout << endl;
	cout << p.toString(OWNER) << endl;

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

	//get the number to bet from user input
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

	unsigned int numToBet = (unsigned int)num;
	unsigned int due;
	if (numToBet > p.bet) {
		due = numToBet - p.bet; //extra chips to pay
	}else {
		due = numToBet;
	}
	
	p.bet += payChips(p, due);
	if (changeBet) bet = numToBet;
	
	return due;
}
