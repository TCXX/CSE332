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

enum Visibility {SEEN_BY_ALL, OWNER_CAN_SEE, NOT_TIL_END, NEVER_SEEN};
enum AccessMode {OTHER, OWNER, ADMIN};

//struct
struct Card {
	CardSuit suit;
	CardRank rank;
	Visibility visible;

	bool operator<(const Card& c) const;
	bool operator==(const Card& c) const;
	string toString(AccessMode mode) const;
	bool isVisible(AccessMode mode) const;
};

#endif //CARD_H_