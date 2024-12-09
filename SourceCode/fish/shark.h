#ifndef Shark_H_INCLUDED
#define Shark_H_INCLUDED


#include "Fish.h"

// fixed settings: MonsterCaveMan attributes
class Shark : public Fish
{
    public:
	Shark(const std::vector<Point> &path) : Fish{path, FishType::Shark} {
		
		v = 40;
		money = 200;
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1})); // swim
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1})); // up
		
		bitmap_switch_freq = 20;
	}
};


#endif


