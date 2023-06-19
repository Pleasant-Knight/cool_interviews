#pragma once

#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include "Player.h"


using namespace std;

// When the house flips the first card.
const int Critical_Threshold = 16;

// House is a special player.
class House : public BasePlayer {
public:
	House(const string& name = "House");
	
	virtual ~House();
	
	virtual bool IsHitting() const;

	void FlipFirstCard();
};

#endif // HOUSE_H
