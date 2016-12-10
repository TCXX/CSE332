/*
Hand.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#include "stdafx.h"
#include "Hand.h"

using namespace std;

//A copy constructor that initializes itself to contain the same sequence of cards.
Hand::Hand(const Hand& h) {
	cards = h.getCards();
}

//A destructor: the compiler-supplied destructor is sufficient.

//Return the card content of the hand.
vector<Card> Hand::getCards() const {
	return cards;
}

//Return the number of cards.
size_t Hand::size() const {
	return cards.size();
}

//Return a space-separated sequence of cards if possible.
string Hand::toString(AccessMode mode) const {
	ostringstream out = ostringstream();
	size_t len = cards.size();
	if (len==0) {
		out << "The hand is empty. ";
	}else {
		size_t hidden = 0;
		for (size_t i = 0; i < len; i++) {
			if (!cards[i].isVisible(mode)) hidden++;
		}
		for (size_t i = 0; i < len; i++) {
			if (cards[i].isVisible(mode)) {
				out << cards[i].toString(mode) << "  ";
			}
		}
		for (size_t i = 0; i < hidden; i++) {
			out << "**  "; 
		}
		
		if ((len>=5) && (hidden == 0)) {
			int r = rankHand();
			out << "   (" << handRankName[r] << ")";
		}
	}

	return out.str();
}


//An assignment operator that updates itself to contain the given sequence of cards.
Hand& Hand::operator=(const Hand& h) {
	if (cards!=h.getCards()) {
		cards.clear();
		vector<Card> c = h.getCards();
		copy(c.begin(), c.end(), back_inserter(cards));
	}
	return *this;
}

//A const equivalence operator that returns true if and only if exactly the same sequence of cards in both hands.
bool Hand::operator==(const Hand& h) const {
	size_t len = size();
	vector<Card> temp = h.getCards();
	if (len != h.size()) {
		return false;
	}
		
	for (size_t i = 0; i < len; i++) {
		if (!(cards[i] == temp[i])) return false;
	}

	return true;
}

//A const less than operator judging whether the hand should appear before the given hand.
bool Hand::operator<(const Hand& h) const {
	vector<Card> c = h.getCards();
	size_t len1 = cards.size();
	size_t len2 = c.size();
	size_t lm = min(len1, len2);
	for (size_t i = 0; i < lm; i++) {
		if (!(cards[i] == c[i])) return cards[i] < c[i];
	}
	return len1 < len2;
}

//Add given card to the hand.
void Hand::pushCard(const Card& c) {
	cards.push_back(c);
	sort(cards.begin(), cards.end(), less<Card>());
}

//Return a number based on hand's rank and content.
int Hand::rankHand() const {
	if (cards.size() < 5) throw HAND_NOT_COMPLETE;
	return (int)floor(findMaxHash()/1000000);
}

//Recommend whether to discard cards. Only works for a hand with five cards.
vector<bool> Hand::discardIndex() const {
	if (cards.size() < 5) throw HAND_NOT_COMPLETE;
	if (cards.size() > 5) throw TOO_MANY_CARDS;

	int rank=HandRank();

	//rank the hand
	vector<bool> index = {false, false, false, false, false}; //card indices to discard

	if (rank==STRAIGHT || rank==FLUSH ) { 
		//do not discard any
		return index;
	}

	if (rank!=NO_RANK) {
		//discard a card if its suit apprears only once
		vector<size_t> freq = vector<size_t>(14);
		for (size_t i = 0; i < 5; i++) freq[cards[i].rank]++;
		for (size_t i = 0; i < 5; i++) {
			if (freq[cards[i].rank] == 1) index[i] = true;
		}
	}
	else { //no rank
		if (cards[4].rank == A) { //eg. 1234A
			index = {true, true, true, true, false};
		} else { //eg. 12345
			index = {true, true, true, true, true};
		}
	}

	return index;
}

//Find the maximum rank of the hand of all possible combinations of five.
int Hand::findMaxHash() const {
	size_t len = cards.size();
	if (len < 5) throw HAND_NOT_COMPLETE;

	HandRank rank = NO_RANK;
	vector<size_t> selected = vector<size_t>(5);

	// record if there is anything special
	int maxOne = 0;
	int maxPair = 0;
	int maxThree = 0;
	int maxFour = 0;

	//count occurences of each rank and each suit
	vector<size_t> rankFreq = vector<size_t>(14);
	for (size_t i = 0; i < len; i++) rankFreq[cards[i].rank]++;
	vector<size_t> suitFreq = vector<size_t>(5);
	for (size_t i = 0; i < len; i++) suitFreq[cards[i].suit]++;

	//find the highest matched level
	for (size_t i = 1; i < 14; i++) {
		if (rankFreq[i] == 4) maxFour = i;
		else if (rankFreq[i] == 3) maxThree = i;
		else if (rankFreq[i] == 2) maxPair = i;
		else if (rankFreq[i] == 1) maxOne = i;
	}

	//deal with the suit - flush
	bool flush = false;
	for (size_t j = 1; j <= 4; j++) {
		if (suitFreq[j] >= 5) flush = true;
	}

	//deal with the rank - straight
	int maxStraight = 0;
	int desire = cards[len - 1].rank - 1;
	size_t count = 1;
	for (size_t i = len; i > 0; i--) {
		if (desire == (int)cards[i - 1].rank) {
			count++;
			desire--;
			if (count == 5) break;
		}
		else {
			count = 1;
			desire = cards[i - 1].rank - 1;
		}
	}
	if (count == 5) {
		maxStraight = desire + 5;
	}

	//deal with the rank - straight flush
	int maxSF = 0;
	for (size_t j = 1; j <= 4; j++) {
		int desire = cards[len - 1].rank - 1;
		size_t count = 1;
		for (size_t i = len; i > 0; i--) {
			if ((desire == (int)cards[i - 1].rank) && (j == (int)cards[i - 1].suit)) {
				count++;
				desire--;
				if (count == 5) break;
			}
		}
		if ((count == 5) && (desire > maxSF)) {
			maxSF = desire + 5;
		}
	}

	if (maxSF>0) { //eg. 12345 of same suit
		rank = STRAIGHT_FLUSH;
		for (size_t i = 0; i < 5; i++) selected[i] = maxSF - i;
	}
	else if (maxFour>0) { //eg. 11112
		rank = FOUR_OF_A_KIND;
		selected[0] = maxFour;
		rankFreq[maxFour] = 0;
		for (size_t i = 1; i < 14; i++) {
			if (rankFreq[i] >= 1) selected[1] = i;
		}
	}
	else if (flush) {
		rank = FLUSH; //eg. sssss
		int maxHash = 0;
		for (size_t j = 1; j <= 5; j++) {
			vector<int> selected_in_suit; //temp selected cards of this suit
			size_t count = 0; //number of cards of this suit
			for (size_t i = len; i > 0; i--) {
				if (j == (int)cards[i - 1].suit) {
					selected_in_suit.push_back((int)cards[i - 1].rank);
					count++;
				}
			}
			if (count >= 5) { //there exists at least five cards of this suit
				int hash = 0;
				for (size_t i = 0; i <= 4; i++) hash = hash * 13 + selected_in_suit[i];
				if (hash > maxHash) maxHash = hash;
			}
		}
		return FLUSH * 1000000 + maxHash;
	}
	else if (maxStraight>0) {
		rank = STRAIGHT; //eg. 12345
		for (size_t i = 0; i < 5; i++) selected[i] = maxStraight - i;
	}
	else if (maxThree>0) { //eg. 111XX
		selected[0] = maxThree;
		rankFreq[maxThree] = 0;
		for (size_t i = 1; i < 14; i++) {
			if (rankFreq[i] >= 2) {
				selected[1] = i;
				rank = FULL_HOUSE; //eg. 11122
			}
		}
		if (rank != FULL_HOUSE) {
			rank = THREE_OF_A_KIND; //eg. 11123
			for (size_t i = 1; i < 14; i++) {
				if (rankFreq[i] >= 1) selected[1] = i;
			}
			for (size_t i = 1; i < selected[1]; i++) {
				if (rankFreq[i] >= 1) selected[2] = i;
			}
		}
	}
	else if (maxPair>0) { //eg. 11XXX
		selected[0] = maxPair;
		rankFreq[maxPair] = 0;
		for (size_t i = 1; i < 14; i++) {
			if (rankFreq[i] >= 2) {
				selected[1] = i;
				rank = TWO_PAIRS; //eg. 1122X
			}
		}
		rankFreq[selected[1]] = 0;
		if (rank == TWO_PAIRS) {
			for (size_t i = 1; i < 14; i++) {
				if (rankFreq[i] >= 1) selected[2] = i;
			}
		}
		else {
			rank = ONE_PAIR; //eg. 11234
			for (size_t i = 1; i < 14; i++) {
				if (rankFreq[i] >= 1) selected[1] = i;
			}
			for (size_t i = 1; i < selected[1]; i++) {
				if (rankFreq[i] >= 1) selected[2] = i;
			}
			for (size_t i = 1; i < selected[2]; i++) {
				if (rankFreq[i] >= 1) selected[3] = i;
			}
		}
	}
	else {
		rank = NO_RANK;
		for (size_t i = 0; i < 5; i++) selected[i] = (size_t)cards[len - i - 1].rank;
	}

	int ans = 0;
	for (size_t i = 0; i <= 4; i++) ans = ans * 13 + selected[i];
	ans = rank * 1000000 + ans; //the highest digit represents rank
	return ans;

}

//Flip all cards in hand to given visibility.
void Hand::flipCards(Visibility vis) {
	size_t len = cards.size();
	for (size_t i = 0; i < len; i++) cards[i].visible = vis;
}

//A non-member insertion operator that removes the card from the back of the deck, and adds it to the hand.
Hand& operator<<(Hand& h, Deck& d) {
	Card c = d.popCard();
	h.pushCard(c);
	return h;
}

//A non-member insertion operator that prints out space-separated card content.
ostream& operator<<(ostream& out, const Hand& h) {
	out<<h.toString(OTHER);
	return out;
}

//An indexing operator that returns the card at that position in the hand. 
Card& Hand::operator[](const size_t index) {
	if (index >= 0 && index < cards.size()) {
		return cards[index];
	}
	else {
		throw HAND_OUT_OF_RANGE;
	}
}

//Remove the card at given position in the hand. 
void Hand::removeCard(const size_t index) {
	if (index >= 0 && index < cards.size()) {
		cards.erase(cards.begin() + index);
	}
	else {
		throw HAND_OUT_OF_RANGE;
	}
}

//Return whether the first hand ranks higher than the second.
bool pokerRank(const Hand& h1, const Hand& h2) {
	return h1.findMaxHash() > h2.findMaxHash();
}


