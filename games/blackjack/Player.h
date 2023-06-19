#pragma once


#ifndef PLAYER_H
#define PLAYER_H

#include "BasePlayer.h"
#include "Card.h"
#include "Hand.h"


class Player : public BasePlayer {
public:
	Player(const string& name = "");
	~Player();

	virtual bool IsBusted() const;
	virtual bool IsHitting() const;

	// Indicate that the player is busted.
	void Bust() const;

	// Indicate that the palyer won/lost or push.
	void Win() const;
	void Lose() const;
	void Push() const;

private:
	string m_Name;
};

#endif // PLAYER_H
