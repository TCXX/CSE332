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
const vector<string> handRankName{ "No Rank", "One Pair", "Two Pairs", "Three of a Kind", "Straight", "Flush", "Full House", "Four of a Kind", "Straight Flush"};
const vector<string> rankName{ "2","3","4","5","6","7","8","9","10","j","q","k","a" };
const vector<string> suitName{ "C","D","H","S" };

//enums
enum CardSuit { NO_SUCH_SUIT, CLUBS, DIAMONDS, HEARTS, SPADES };
enum CardRank { NO_SUCH_RANK, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, J, Q, K, A};
enum HandRank { NO_RANK, ONE_PAIR, TWO_PAIRS, THREE_OF_A_KIND, STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH};

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