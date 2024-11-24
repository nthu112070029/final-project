#include "hero.h"
#include <cstdio>
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include <string>
#include "shapes/Rectangle.h"
#include "algif5/algif.h"

namespace HeroSetting {
	static constexpr char gif_root_path[40] = 
		"./assets/gif/Hero";
	static constexpr char gif_postfix[][10] = {
		"left", "right", "front", "back"
	};
}
 void Hero::init(){
    for(size_t type=0 ;type<static_cast<size_t>(Herostate::herostatamax);type++){
    char buffer[50];
	sprintf(
		  buffer, "%s/dragonite_%s.gif",
		HeroSetting::gif_root_path,
		  HeroSetting::gif_postfix[static_cast<int>(type)]);
        gifpath[static_cast<Herostate>(type)]=std::string(buffer);
        
    }
    GIFCenter *GIFC = GIFCenter::get_instance();
ALGIF_ANIMATION *gif = GIFC->get(gifpath [state]);
DataCenter *DC = DataCenter::get_instance();
shape.reset(new Rectangle{DC->window_width / 2,
DC->window_height / 2,
DC->window_width / 2 + gif->width,
DC->window_height / 2 + gif->height});
 }
 
		
	void Hero::update()
{
    DataCenter *DC = DataCenter::get_instance();
    if (DC->key_state[ALLEGRO_KEY_W])
    {
        shape->update_center_y(shape->center_y() - speed);
        state = Herostate::back;
    }
    else if (DC->key_state[ALLEGRO_KEY_A])
    {
        shape->update_center_x(shape->center_x() - speed);
        state = Herostate::left;
    }
    else if (DC->key_state[ALLEGRO_KEY_S])
    {
        shape->update_center_y(shape->center_y() + speed);
        state = Herostate::front;
    }
    else if (DC->key_state[ALLEGRO_KEY_D])
    {
        shape->update_center_x(shape->center_x() + speed);
        state = Herostate::right;
    }
}

	void Hero::draw(){
       
GIFCenter *GIFC = GIFCenter::get_instance();
ALGIF_ANIMATION *gif = GIFC->get(gifpath [state]);
algif_draw_gif(gif,
shape->center_x() - gif->width / 2,
shape->center_y() - gif->height / 2, 0);



    }