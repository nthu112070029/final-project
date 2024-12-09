#include "Player.h"

// fixed settings
namespace PlayerSetting {
	constexpr int init_coin = 200;
	constexpr int time_freq = 60;
};

Player::Player() : coin(PlayerSetting::init_coin) {
	this->time_freq = PlayerSetting::time_freq;
	time_counter = PlayerSetting::time_freq;
}

void
Player::update() {
	if(time_counter) --time_counter;
	else {
		countdown--;
		
		time_counter = time_freq;
	}
}
