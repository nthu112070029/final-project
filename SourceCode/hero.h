#ifndef hero_H_INCLUDED
#define hero_H_INCLUDED
#include "Object.h"
#include <string>
#include <map>
enum class Herostate{
	left, right, front,herostatamax
};
class Hero : public Object
{
    public:
    void init();
	void update();
	void draw();
    private:
    Herostate state=Herostate::front;
    double speed=5;
    std::map<Herostate ,std::string> gifpath;

};


#endif