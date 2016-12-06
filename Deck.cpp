/*
Deck.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#include "stdafx.h"
#include "Deck.h"


using namespace std;

//A constructor.
Deck::Deck() {
	cards = vector<Card>();
}

//A destructor: the compiler-supplied destructor is sufficient.
Deck::~Deck() {
	//No further action is needed as of now, therefore leaves the destructor for future implementation
}

//To return private cards variable even for outside of the class.
vector<Card> Deck::getCards() const {
	return cards;
}

//To pop and return the last card in the deck.
Card Deck::popCard() {
	if (cards.size()==0) throw(EMPTY_HAND);
	Card c = cards.back();
	cards.pop_back();
	return c;
}

//An add_card method that takes a card as its only parameter, and adds it to the deck.
void Deck::addCard(Card& c) {
	cards.push_back(c);
}

//A "shuffle" method that randomizes the order of all the cards.
void Deck::shuffle() {
	srand((unsigned int)time(0));
	std::shuffle(cards.begin(), cards.end(), default_random_engine());
}

//A const "size" method that returns the number of elements in the container member variable.
size_t Deck::size() const {
	return cards.size();
}

//To print out deck's information, one card per line.
string Deck::toString() const {
	ostringstream out = ostringstream();
	size_t len = cards.size();
	
	if (len==0) {
		out << "The deck is empty. " << endl;
	}else {
		out << "The deck has " << len << " cards: " << endl;
		for (size_t i = 0; i < len; i++) {
			CardSuit suit = cards[i].suit;
			CardRank rank = cards[i].rank;
			out << rankName[rank - 1] << suitName[suit - 1] << "  ";
			if ((i + 1) % 5 == 0) out << endl;
		}
		if (len % 5 != 0) out << endl;
	}
	return out.str();
}

//To read a card's suit from a string.
CardSuit Deck::convertSuit(const char& input) {
	switch (input) {
	case 'S': case 's': return SPADES;
	case 'C': case 'c': return CLUBS;
	case 'D': case 'd': return DIAMONDS;
	case 'H': case 'h': return HEARTS;
	default: return NO_SUCH_SUIT;
	}
}

//To read a card's rank from a string.
CardRank Deck::convertRank(const char& input) {
	switch (input) {
	case 'A': case 'a': return A;
	case '2': return TWO;
	case '3': return THREE;
	case '4': return FOUR;
	case '5': return FIVE;
	case '6': return SIX;
	case '7': return SEVEN;
	case '8': return EIGHT;
	case '9': return NINE;
	case '1': return TEN;
	case 'J': case 'j': return J;
	case 'Q': case 'q': return Q;
	case 'K': case 'k': return K;
	default: return NO_SUCH_RANK;
	}
}

//A function that fills the deck with standard 52 cards.
void Deck::standardized() {
	cards = vector<Card>();
	for (int i = 1; i < 5; i++) {
		for (int j = 1; j < 14; j++) {
			CardSuit suit = static_cast<CardSuit>(i);
			CardRank rank = static_cast<CardRank>(j);
			Card c = { suit, rank, true };
			addCard(c);
		}
	}
	shuffle();
}

void Deck::flipCards(bool faceUp) {
	size_t len = cards.size();
	for (size_t i = 1; i < len; i++) cards[i].visible = faceUp;
}

//A non-member insertion operator (operator<<) that prints out valid card definition strings on separate lines.
ostream& operator<<(ostream& out, const Deck& d) {
	out << d.toString();
	return out;
}
