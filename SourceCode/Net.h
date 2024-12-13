#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include "Object.h"
#include <allegro5/bitmap.h>
#include <string>

enum class Netstate{

   up,down
};

class Net : public Object
{

	
    public:
     void init();
	void update();
	void draw();
	double vx=0;
    double vy=100;
    double speed=5.0;
   
    static bool show;
    Netstate state;
	ALLEGRO_BITMAP *bitmap;
    int reachbottom=0;
    double x;
    double y;
};

#endif