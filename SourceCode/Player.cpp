#include "Player.h"

// fixed settings
namespace PlayerSetting {
	constexpr int init_HP = 3;
	
	constexpr int init_coin = 100;
	constexpr int coin_freq = 60;
	constexpr int coin_increase = 5;
	constexpr int time_freq = 60;
};

Player::Player() : HP(PlayerSetting::init_HP), coin(PlayerSetting::init_coin) {
	this->coin_freq = PlayerSetting::coin_freq;
	this->time_freq = PlayerSetting::time_freq;
	this->coin_increase = PlayerSetting::coin_increase;
	coin_counter = PlayerSetting::coin_freq;
	time_counter = PlayerSetting::time_freq;
}

void
Player::update() {
	if(coin_counter) --coin_counter;
	else {
		countdown--;
		coin += coin_increase;
		coin_counter = coin_freq;
	}
}
