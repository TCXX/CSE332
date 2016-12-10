/*
Hand.h created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#pragma once
#ifndef HAND_H_
#define HAND_H_

//#include "stdafx.h"
#include "Card.h"
#include "Deck.h"
#include "lab.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;
class Deck;
struct Player;

//hand rank
enum HandRank { NO_RANK, ONE_PAIR, TWO_PAIRS, THREE_OF_A_KIND, STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH };
const vector<string> handRankName{ "No Rank", "One Pair", "Two Pairs", "Three of a Kind",
								   "Straight", "Flush", "Full House", "Four of a Kind", "Straight Flush" };

//A class that represents a card player's hand of cards.
class Hand{
	public:
		Hand() = default;
		Hand(const Hand& h);

		size_t size() const;
		string toString(AccessMode mode) const;

		vector<Card> getCards() const;
		void pushCard(const Card& c);
		int rankHand() const;
		vector<bool> Hand::discardIndex() const;
		int Hand::findMaxHash() const;
		void removeCard(const size_t index);
		void flipCards(Visibility vis);

		//operators
		Hand& operator=(const Hand& h);
		bool operator==(const Hand& h) const;
		bool operator<(const Hand& h) const;
		Card& operator[] (const size_t index);
	
	private:
		vector<Card> cards;

};

Hand& operator<<(Hand& h, Deck& d);
ostream& operator<<(ostream& out, const Hand& h);
bool pokerRank(const Hand& h1, const Hand& h2);

#endif //HAND_H_
