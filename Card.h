/*
Card.h created by Cindy Le (lex@wustl.edu)
*/

#pragma once
//To avoid multiple inclusion
#ifndef CARD_H_
#define CARD_H_

#include "stdafx.h"
#include <vector>
#include <string>

using namespace std;

//consts
const vector<string> rankName{ "2","3","4","5","6","7","8","9","10","j","q","k","a" };
const vector<string> suitName{ "C","D","H","S" };

//enums
enum CardSuit { NO_SUCH_SUIT, CLUBS, DIAMONDS, HEARTS, SPADES };
enum CardRank { NO_SUCH_RANK, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, J, Q, K, A};

//struct
struct Card {
	CardSuit suit;
	CardRank rank;
	bool visible;

	bool operator<(const Card& c) const;
	bool operator==(const Card& c) const;
	string toString(bool owner) const;
};

#endif //CARD_H_