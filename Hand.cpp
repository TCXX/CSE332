/*
Hand.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#include "stdafx.h"
#include "Hand.h"

using namespace std;

//A copy constructor that takes a reference to another const hand object and initializes itself to contain the same sequence.
Hand::Hand(const Hand& h) {
	cards = h.getCards();
}

//A destructor: the compiler-supplied destructor is sufficient.
Hand::~Hand() {
	//No further action is needed as of now, therefore leaves the destructor for future implementation
}

//A const "get cards" method that returns the cards of the hand in the container
vector<Card> Hand::getCards() const {
	return cards;
}

//A const "size" method that returns the number of elements in the container member variable.
size_t Hand::size() const {
	return cards.size();
}

//A const "as string" method that returns (by value) a C++ style string containing a space-separated sequence of valid card definition.
string Hand::toString() const {
	ostringstream out = ostringstream();
	size_t len = cards.size();
	if (len==0) {
		out << "The hand is empty. ";
	}else {
		
		for (size_t i = 0; i < len; i++) {
			CardSuit suit = cards[i].suit;
			CardRank rank = cards[i].rank;
			out << rankName[rank - 1] << suitName[suit - 1] << "  ";
		}
		if (len == 5) {
			int r = rankHand();
			out <<"   ("<<handRankName[r]<<")";
		}
	}

	return out.str();
}


//An assignment operator that clears out existing cards and updates itself to contain the same sequence of cards as in the passed object.
Hand& Hand::operator=(const Hand& h) {
	if (cards!=h.getCards()) {
		cards.clear();
		vector<Card> c = h.getCards();
		copy(c.begin(), c.end(), back_inserter(cards));
	}
	return *this;
}

//A const equivalence operator that returns true if and only if exactly the same sequence of cards appears in both objects.
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

//A const less than operator judging whether the hand should appear before the given hand according to a lexical (phone book style) ordering.
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

//To add the given card to the hand.
void Hand::pushCard(const Card& c) {
	cards.push_back(c);
	sort(cards.begin(), cards.end(), less<Card>());
}

//To return a number representing the hand's rank.
int Hand::rankHand() const {
	return (int)floor(hashHand()/1000000);
}

//To find rank for a hand, and return a 7-digit hash value for it.
int Hand::hashHand() const  {
	if (cards.size() < 5) throw HAND_NOT_COMPLETE;

	//deal with same rank
	bool pair[4];
	bool three[3];
	bool four[2];

	for (size_t i = 0; i <= 3; i++) {
		if (cards[i].rank == cards[i + 1].rank) pair[i] = true;
		else pair[i] = false;
	}

	for (size_t i = 0; i <= 2; i++) three[i] = pair[i] && pair[i + 1];
	for (int i = 0; i <= 1; i++) four[i] = three[i] && three[i + 1];

	//deal with consecutive rank
	bool straight = true;
	for (size_t i = 0; i <= 3; i++) {
		if (cards[i + 1].rank - cards[i].rank != 1) straight = false;
	}

	//deal with same suit
	bool flush = true;
	for (size_t i = 0; i <= 3; i++) {
		if (cards[i + 1].suit != cards[i].suit) flush = false;
	}

	//rank the hand
	HandRank rank;
	vector<size_t> index(5); //specify the rule to determine orders when two hands have same rank

	if (straight && flush) { //eg. 12345 all same suit
		rank = STRAIGHT_FLUSH;
		index = { 5, 9, 9, 9, 9 };
	}
	else if (four[0]) { //eg. 11112
		rank = FOUR_OF_A_KIND;
		index = { 3, 4, 9, 9, 9 };
	}
	else if (four[1]) { //eg. 12222
		rank = FOUR_OF_A_KIND;
		index = { 4, 0, 9, 9, 9 };
	}
	else if (three[0] && pair[3]) { //eg. 11122
		rank = FULL_HOUSE;
		index = { 2, 4, 9, 9, 9 };
	}
	else if (three[2] && pair[0]) { //eg. 11222
		rank = FULL_HOUSE;
		index = { 4, 1, 9, 9, 9 };
	}
	else if (flush) { //all same suit
		rank = FLUSH;
		index = { 4, 3, 2, 1, 0 };
	}
	else if (straight) { //eg. 12345
		rank = STRAIGHT;
		index = { 4, 3, 2, 1, 0 };
	}
	else if (three[0]) { //eg. 11123
		rank = THREE_OF_A_KIND;
		index = { 2, 4, 3, 9, 9 };
	}
	else if (three[1]) { //eg. 12223
		rank = THREE_OF_A_KIND;
		index = { 3, 4, 0, 9, 9 };
	}
	else if (three[2]) { //eg. 12333
		rank = THREE_OF_A_KIND;
		index = { 4, 1, 0, 9, 9 };
	}
	else if (pair[0] && pair[2]) { //eg. 11223
		rank = TWO_PAIRS;
		index = { 3, 1, 4, 9, 9 };
	}
	else if (pair[1] && pair[3]) { //eg. 12233
		rank = TWO_PAIRS;
		index = { 4, 2, 0, 9, 9 };
	}
	else if (pair[0] && pair[3]) { //eg. 11233
		rank = TWO_PAIRS;
		index = { 4, 1, 2, 9, 9 };
	}
	else if (pair[0]) { //eg. 11234
		rank = ONE_PAIR;
		index = { 1, 4, 3, 2, 9 };
	}
	else if (pair[1]) { //eg. 12234
		rank = ONE_PAIR;
		index = { 2, 4, 3, 0, 9 };
	}
	else if (pair[2]) { //eg. 12334
		rank = ONE_PAIR;
		index = { 3, 4, 1, 0, 9 };
	}
	else if (pair[3]) { //eg. 12344
		rank = ONE_PAIR;
		index = { 4, 2, 1, 0, 9 };
	}
	else {
		rank = NO_RANK;
		index = { 4, 3, 2, 1, 0 };
	}

	int ans = 0;
	for (size_t i = 0; i <= 4; i++) if (index[i] <= 4) ans = ans * 13 + cards[index[i]].rank;
	ans = rank * 1000000 + ans; //the highest digit represents rank
	return ans;
}

vector<bool> Hand::discardIndex() const {
	if (cards.size() < 5) throw HAND_NOT_COMPLETE;

	//deal with same rank
	bool pair[4];
	bool three[3];
	bool four[2];

	for (size_t i = 0; i <= 3; i++) {
		if (cards[i].rank == cards[i + 1].rank) pair[i] = true;
		else pair[i] = false;
	}

	for (size_t i = 0; i <= 2; i++) three[i] = pair[i] && pair[i + 1];
	for (size_t i = 0; i <= 1; i++) four[i] = three[i] && three[i + 1];

	//deal with consecutive rank
	bool straight = true;
	for (size_t i = 0; i <= 3; i++) {
		if (cards[i + 1].rank - cards[i].rank != 1) straight = false;
	}

	//deal with same suit
	bool flush = true;
	for (size_t i = 0; i <= 3; i++) {
		if (cards[i + 1].suit != cards[i].suit) flush = false;
	}

	//rank the hand
	vector<bool> index; //card indices to discard

	if (straight  || flush || (three[0] && pair[3]) || (three[2] && pair[0])) { 
		index = { false, false, false, false, false };
	}
	else if (four[0]) { //eg. 11112
		index = {false, false, false, false, true};
	}
	else if (four[1]) { //eg. 12222
		index = {true, false, false, false, false};
	}
	else if (three[0]) { //eg. 11123
		index = {false, false ,false ,true ,true};
	}
	else if (three[1]) { //eg. 12223
		index = {true, false, false ,false, true};
	}
	else if (three[2]) { //eg. 12333
		index = {true, true, false, false, false};
	}
	else if (pair[0] && pair[2]) { //eg. 11223
		index = {false, false, false, false, true};
	}
	else if (pair[1] && pair[3]) { //eg. 12233
		index = {true, false, false, false, false};
	}
	else if (pair[0] && pair[3]) { //eg. 11233
		index = {false, false, true, false, false};
	}
	else if (pair[0]) { //eg. 11234
		index = {false,false, true, true, true};
	}
	else if (pair[1]) { //eg. 12234
		index = {true, false, false, true, true};
	}
	else if (pair[2]) { //eg. 12334
		index = {true, true, false ,false, true};
	}
	else if (pair[3]) { //eg. 12344
		index = {true, true, true, false, false};
	}
	else { //no rank
		if (cards[4].rank == A) { //eg. 1234A
			index = {true, true, true, true, false};
		} else { //eg. 12345
			index = { true, true, true, true, true};
		}
	}

	return index;
}


//A non-member insertion operator (operator<<) that removes the card from the back of the deck, and adds it to the hand.
Hand& operator<<(Hand& h, Deck& d) {

	Card c = d.popCard();
	h.pushCard(c); //pushCard() guarantees its sorted order
	return h;
}

//A non-member insertion operator (operator<<) that prints out space-separated valid card definition strings on the same line.
ostream& operator<<(ostream& out, const Hand& h) {
	out<<h.toString();
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

//A remove_card method that removes the card at that position in the hand. 
void Hand::removeCard(const size_t index) {
	if (index >= 0 && index < cards.size()) {
		cards.erase(cards.begin() + index);
	}
	else {
		throw HAND_OUT_OF_RANGE;
	}
}

//A "poker_rank" function judging whether the first hand ranks higher than the second hand.
bool pokerRank(const Hand& h1, const Hand& h2) {
	return h1.hashHand() > h2.hashHand();
}


