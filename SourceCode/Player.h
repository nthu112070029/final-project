#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

class Player
{
public:
	Player();
	void update();
	int coin;
	int countdown;
	int goal;
private:
	int time_freq;
	int time_counter;
};

#endif
