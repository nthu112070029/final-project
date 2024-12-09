#ifndef FishingRod_H_INCLUDED
#define FishingRod_H_INCLUDED

#include "../Object.h"
#include "../shapes/Rectangle.h"
#include <allegro5/bitmap.h>
#include <string>
#include <array>
#include<map>

class Hook;

// fixed settings
enum class RodType {
	oringin, plus,maxtype
};
enum class Rodstate {
	up,down,maxstate
};
class Rod : public Object
{
	
public:
    void init();
	void update();
	void draw();
	RodType get_type() const { return this->type; }
	RodType type=RodType::oringin;
	std::map<RodType ,std::map<Rodstate,std::string>> imgpath;
	 double speed=5;
	 
	 int damage;
	  double x;
	  double y;
	ALLEGRO_BITMAP *bitmap;	
    Rodstate state=Rodstate::up;
	
};

#endif