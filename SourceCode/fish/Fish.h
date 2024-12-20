#ifndef Fish_H_INCLUDED
#define Fish_H_INCLUDED

#include "../Object.h"
#include "../shapes/Rectangle.h"
#include <vector>
#include <queue>

enum class Fishstate;

// fixed settings
enum class FishType {
	Salmon,Snapper,Tuna,Shark, FishTYPE_MAX
};


class Fish : public Object
{
public:
	static Fish *create_Fish(FishType type, const std::vector<Point> &path);
public:
	Fish(const std::vector<Point> &path, FishType type);
	void update();
	void draw();
	const int &get_money() const { return money; }
	
	FishType get_type() const { return this->type; }
	const std::queue<Point> &get_path() const { return path; }
	void set_v (int V){v=V;}
protected:
	
	int v;
	int money;
	std::vector<std::vector<int>> bitmap_img_ids;
	int bitmap_switch_counter;
	int bitmap_switch_freq;
	int bitmap_img_id;
private:
	FishType type;
	Fishstate state;
	std::queue<Point> path;
	int timecounter=60;
	int timefreq=60;
	int stilltime=5;
};

#endif
