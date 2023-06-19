#pragma once

#ifndef HAND_H
#define HAND_H

#include "Card.h"


class Hand {
public:
	// Defualt Ator/Dtors (virtual).
	Hand();
	virtual ~Hand();
	
	// Clears hand of all cards.
	void Clear();
	
	// Add Card to the current hand.
	void Add(Card *card);
	
	// Gets current hand total value.
	// Can treat Aces as 1 or 11.
	int GetTotal() const;

protected:
	vector<Card*> m_Cards;
};


#endif // HAND_H
