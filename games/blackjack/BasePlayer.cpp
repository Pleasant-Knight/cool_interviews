#include "BasePlayer.h"

BasePlayer::BasePlayer(const string& name) : m_Name(name) {}

BasePlayer::~BasePlayer() {}


bool BasePlayer::IsBusted() const {
	return (GetTotal() > 21);
}

void BasePlayer::Bust() const {
	cout << "Base Player: " << m_Name << " is busted.\n";
}

