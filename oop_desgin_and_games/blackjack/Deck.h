#pragma once

#ifndef DECK_H
#define DECK_H

#include <vector>
#include "Hand.h"
#include "BasePlayer.h"

using namespace std;

class Deck : public Hand {
public:
	Deck();

	virtual ~Deck();
	
	// create random deck.
	void Populate();

	// shuffle the dec.
	void Shuffle();
	
	// deal one card to a hand.
	void Deal(Hand& hand);
	
	// give more cards to player.
	void MoreCards(BasePlayer& player);
};


#endif // DECK_H
