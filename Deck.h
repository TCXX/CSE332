/*
Deck.h created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#pragma once
//To avoid multiple inclusion
#ifndef DECK_H_
#define DECK_H_

//#include "stdafx.h"
#include "Card.h"
#include "Hand.h"

#include <vector>
#include <string>
#include <fstream>

//for randomization
#include <ctime>
#include <cstdlib>
#include <random>

//for std::copy
#include <iterator> 

using namespace std;
class Hand;

class Deck{
	public:
		Deck();
		~Deck();

		vector<Card> getCards() const;
		Card popCard();
		void shuffle();
		void addCard(Card& c);
		void standardized();
		void flipCards(bool faceUp);
		size_t size() const;
		string toString() const;

	private:
		vector<Card> cards;
		CardSuit convertSuit(const char& input);
		CardRank convertRank(const char& input);

};

ostream& operator<<(ostream& out, const Deck& d);

#endif //Deck_H_
