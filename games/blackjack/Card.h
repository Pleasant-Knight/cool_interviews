#pragma once

#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

//namespace Card {

class Card {
public:
	enum rank {
		ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
	};

	enum suit { Clubs, Diamonds, Hearts, Spades };

	// The stream operator.
	friend ostream& operator << (ostream& os, const Card& card);

	Card(rank r = ACE, suit s = Spades, bool ifu = true); // is face up?

	// Returns the value of the card.
	int GetValue() const;

	// Flips the card face up or down.
	void Flip();

private:
	rank m_Rank;
	suit m_Suit;
	bool m_isFaceUp;
};

//}

#endif // CARD_H