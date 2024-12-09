#ifndef Snapper_H_INCLUDED
#define Snapper_H_INCLUDED


#include "Fish.h"

// fixed settings: MonsterCaveMan attributes
class Snapper : public Fish
{
    public:
	Snapper(const std::vector<Point> &path) : Fish{path, FishType::Snapper} {
		
		v = 40;
		money = 20;
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1})); // swim
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1})); // up
		
		bitmap_switch_freq = 20;
	}
};


#endif


