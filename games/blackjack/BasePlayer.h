#pragma once


#ifndef BASE_PLAYER_H
#define BASE_PLAYER_H


#include "card.h"
#include "hand.h"

/*
=================================================
Generic Player class
=================================================
*/
class BasePlayer : public Hand {
	// The ostream operator.
	friend ostream& operator << (ostream& os, const BasePlayer& aBasePlayer);

public:
	BasePlayer(const string& name = "");

	virtual ~BasePlayer();

	// Indicates wheater a player wants to keep hitting or not.
	virtual bool IsHitting() const = 0;

	// Returns wheater a player is busted or not.
	// Note: busted means that player has total > 21.
	bool IsBusted() const;

	// Annouces that the generic player bust.
	void Bust() const;

protected:
	string m_Name;
};


#endif // BASE_PLAYER_H
