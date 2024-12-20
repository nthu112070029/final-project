#ifndef SALMON_H_INCLUDED
#define SALMON_H_INCLUDED


#include "Fish.h"

// fixed settings: MonsterCaveMan attributes
class Salmon : public Fish
{
    public:
	Salmon(const std::vector<Point> &path) : Fish{path, FishType::Salmon} {
		
		v = 40;
		money = 40;
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1})); // swim
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1})); // up
		
		bitmap_switch_freq = 20;
	}
};


#endif


