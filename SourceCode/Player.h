#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

class Player
{
public:
	Player();
	void update();
	int HP;
	int coin;
	int countdown=60;
private:
	int coin_freq;
	int coin_increase;
	int coin_counter;
	int time_freq;
	int time_counter;
	int lvl;
};

#endif
