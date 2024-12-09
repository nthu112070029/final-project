#include "Rod.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include <allegro5/bitmap_draw.h>
#include <allegro5/allegro_primitives.h>
namespace RodSetting {
        static constexpr char rod_imgs_root_path[static_cast<int>(RodType::maxtype)][40] = {
      "./assets/image/Rod/oringin",
	    "./assets/image/Rod/plus"
	};
	static constexpr char state_path_prefix[][10] = {
		"UP", "DOWN"
	};

};


void Rod::init(){
   type=RodType::oringin;
   for(size_t type=0 ;type<static_cast<size_t>(RodType::maxtype);type++){
        for(size_t state=0 ;state<static_cast<size_t>(Rodstate::maxstate);state++){
        char buffer[50];
	    sprintf(
		    buffer, "%s/%s.png",
		    RodSetting::rod_imgs_root_path[static_cast<int>(type)],
		    RodSetting::state_path_prefix[static_cast<int>(state)]);
            imgpath[static_cast<RodType>(type)][static_cast<Rodstate>(state)]=std::string(buffer);
    }}
    switch(type){
      case RodType::oringin:
         damage = 30;
         break;
      case RodType::plus:
         damage = 50;
         break;
      default:
         damage = 20;
    }
    ImageCenter *IC = ImageCenter::get_instance();
    ALLEGRO_BITMAP *bitmap=IC->get(imgpath [type][state]);
    DataCenter *DC = DataCenter::get_instance();
    shape.reset(new Rectangle{DC->window_width / 2,
    100,
    DC->window_width / 2 - al_get_bitmap_width(bitmap),
    DC->window_height*0 + al_get_bitmap_height(bitmap)});

 }
 
		
void Rod::update()
{
   
   DataCenter *DC = DataCenter::get_instance();
   switch(state)
   {
        case Rodstate::up:
             if (DC->key_state[ALLEGRO_KEY_A]||DC->key_state[ALLEGRO_KEY_LEFT])
             {
                shape->update_center_x(shape->center_x() - speed);
                state = Rodstate::up;
                return ;
            }
            else if (DC->key_state[ALLEGRO_KEY_D]||DC->key_state[ALLEGRO_KEY_RIGHT])
            {
                shape->update_center_x(shape->center_x() + speed);
                state = Rodstate::up;
                  return ;
            }
            else if(DC->key_state[ALLEGRO_KEY_S]||DC->key_state[ALLEGRO_KEY_DOWN])
            {

                state=Rodstate::down;
                return ;
            }
         case Rodstate::down:
            state=Rodstate::up;
           case Rodstate::maxstate:
           return;


   }

   
}

void Rod::draw(){
       
   ImageCenter *IC = ImageCenter::get_instance();
    ALLEGRO_BITMAP *bitmap=IC->get(imgpath [type][state]);
    
    al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
       x=shape->center_x()+(al_get_bitmap_width(bitmap) / 2);
      y=shape->center_y()-(al_get_bitmap_height(bitmap) / 2);
}
//
