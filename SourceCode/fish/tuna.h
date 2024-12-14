#ifndef Tuna_H_INCLUDED
#define Tuna_H_INCLUDED


#include "Fish.h"

// fixed settings: MonsterCaveMan attributes
class Tuna : public Fish
{
    public:
	Tuna(const std::vector<Point> &path) : Fish{path, FishType::Tuna} {
	
		v = 40;
		money = 50;
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1})); // swim
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1})); // up
		
		bitmap_switch_freq = 20;
	}
};


#endif


