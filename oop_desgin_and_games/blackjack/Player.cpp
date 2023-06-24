#include "Player.h"

Player::Player(const string& name) : m_Name(name) {}

Player::~Player() {}

// If the player has over 21 and is busted.
bool Player::IsBusted() const {
	return (GetTotal() > 21);
}

bool Player::IsHitting() const {
	cout << "\nPlayer "<< m_Name << ", do you want a hit (Y/N): ";
	char response;
	cin >> response;

	return (response == 'y' || response == 'Y');

}

// Say that the player is busted
void Player::Bust() const {
	cout << "Player: " << m_Name << " is busted!\n";
}


void Player::Win() const {
	cout << "\nPlayer: " << m_Name << " has WON :)\n";
}

void Player::Lose() const {
	cout << "\nPlayer: " << m_Name << " has LOST!\n";
}

void Player::Push() const {
	cout << "\nPlayer: " << m_Name << " pushes.\n";
}
