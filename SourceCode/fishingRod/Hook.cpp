#include "Hook.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Point.h"
#include <algorithm>
#include <allegro5/bitmap_draw.h>
#include <iostream>

bool Hook::fishcaught = false;

void Hook::init()
{
     type=Hooktype::oringin;
     state=Hookstate::up;
    
ImageCenter *IC = ImageCenter::get_instance();
    bitmap = IC->get("./assets/image/Hook/origin.png");
	
	 DataCenter *DC = DataCenter::get_instance();
    shape.reset(new Rectangle{DC->window_width / 2,
    150,
    DC->window_width / 2 - al_get_bitmap_width(bitmap),
    DC->window_height*0 + al_get_bitmap_height(bitmap)});

}
void
Hook::update() {
 DataCenter *DC = DataCenter::get_instance();

   
   switch(state)
   {
           
            case Hookstate::up:
             if (DC->key_state[ALLEGRO_KEY_A]||DC->key_state[ALLEGRO_KEY_LEFT])
             {
                shape->update_center_x(shape->center_x() - speed);
                state = Hookstate::up;
                break;
            }
            else if (DC->key_state[ALLEGRO_KEY_D]||DC->key_state[ALLEGRO_KEY_RIGHT])
            {
                shape->update_center_x(shape->center_x() + speed);
                state = Hookstate::up;
                break;
            }
            else if(DC->key_state[ALLEGRO_KEY_S]||DC->key_state[ALLEGRO_KEY_DOWN])
            {
                state=Hookstate::down;
                break;
            }
            else if(shape->center_y()<DC->window_height/7){
                vy=100;//出水面碰到人鉤子轉向
                state = Hookstate::up;
                fishcaught=false;
            }  
            break;
             case Hookstate::down:
            {
                 
                static int run=0;
	             double dy = vy / DC->FPS;
               
               
            if(shape->center_y()>DC->window_height-50||fishcaught  )
            {       
                   
                    vy = -100;
                    dy = vy / DC->FPS;
                    run=1;
                  
                   
                                        
            }
             if(shape->center_y()<DC->window_height/7 && run){
               
                        vy=100;
                        fishcaught=false;
                        run=0;
                        
                          state = Hookstate::up;
                          break;   
            }   
	                shape->update_center_y(shape->center_y() + dy);
                    return; 
                    
            }
        

   }
}

void
Hook::draw() {
    ImageCenter *IC = ImageCenter::get_instance();
    if(type==Hooktype::oringin)
    {
        bitmap = IC->get("./assets/image/Hook/origin.png");
    }
    else{
        bitmap = IC->get("./assets/image/Hook/plus.png");
        
    }
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
         x=shape->center_x();
        y=shape->center_y()-(al_get_bitmap_height(bitmap) / 2);
       
}
