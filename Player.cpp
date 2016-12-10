/*
Player.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#include "stdafx.h"
#include "Player.h"

using namespace std;

//A constructor that tries to read data from a file that has the same name as the player.
Player::Player(string playername, bool aut):
		name(playername), won(0), lost(0), chip(20), bet(0), isFold(false), isAuto (aut) {
	hand = Hand();
	
	string filename = name + ".txt"; //name is stored without '*'
	ifstream ifs(filename);
	if (ifs.is_open()) {
		string line;
		vector<string> temp = vector<string>(); //to store readins

		//readin strings
		while (!ifs.eof()) {
			getline(ifs, line);
			temp.push_back(line);
		}

		//parse if valid
		if (temp.size()>=4) {
			//name = temp[0]; don't need because name will not be changed
			won = stoi(temp[1]);
			lost = stoi(temp[2]);
			chip = stoi(temp[3]);
		}	
	}
}

//Print out player's information in a line.
string Player::toString(AccessMode mode) const {
	ostringstream out = ostringstream();

	//basic info
	out << "Player " << name;
	if (isAuto) out << "*";
	out << "(" << chip << "): ";

	//hand content
	if (isFold) {
		int len = hand.size();
		for (int i = 0; i < len; i++) out << "**  ";
	}
	else {
		out << hand.toString(mode);
	}

	return out.str();
}

//Reset chips for this player.
int Player::reset() {
	chip = 20;
	return 0;
}

//A non-member insertion operator that prints out player's information.
ostream& operator<<(ostream& out, const Player& p) {
	out << p.toString(OTHER);
	return out;
}

//A non-member insertion operator that prints out player data to a txt file.
ofstream& operator<<(ofstream& out, const Player& p) {
	out << p.name << endl;
	out << p.won << endl;
	out << p.lost << endl;
	return out;
}

//Return if the first player has a better position in a sorted list from high to low.
bool playerRank(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2) {
	if (p1.get() == NULL) return false;
	if (p2.get() == NULL) return true;
	if (p1->isFold) return false;
	if (p2->isFold) return true;
	return p1->hand.findMaxHash() > p2->hand.findMaxHash();
}

