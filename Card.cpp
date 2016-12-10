/*
Card.cpp created by Cindy Le (lex@wustl.edu)
*/

#include "stdafx.h"
#include "Card.h"

using namespace std;

//Return if the first card is smaller than the second.
bool Card::operator<(const Card& c) const {
	//compares for by rank then suit
	return rank * 4 + suit < c.rank * 4 + c.suit;
}

//Return if two cards are identical.
bool Card::operator==(const Card& c) const {
	//compares for by rank then suit
	return rank * 4 + suit == c.rank * 4 + c.suit;
}

//Return card content and hide based on access mode.
string Card::toString(AccessMode mode) const {
	if (!isVisible(mode)) return "** ";
	string str = rankName[rank - 1] + suitName[suit - 1];
	return str;
}

//Return whether card content should be visible under given access mode.
bool Card::isVisible(AccessMode mode) const {
	int intMode = (int)mode;
	int intVis = (int)visible;
	if (intVis > intMode) return false;
	return true;
}

