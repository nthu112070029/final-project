#ifndef HOOK_H_INCLUDED
#define HOOK_H_INCLUDED

#include "../Object.h"
#include <allegro5/bitmap.h>
#include <string>
enum class Hooktype{

    oringin, plus
};
enum class Hookstate{

   up,down
};

class Hook : public Object
{
public:
	
    void init();
	void update();
	void draw();
	double vx=0;
    double vy=100;
    double speed=5.0;
    static bool fishcaught;//魚有沒有被抓到
	Hooktype type;
    Hookstate state;
	ALLEGRO_BITMAP *bitmap;
    int reachtop=0;
    double x;
    double y;
};

#endif