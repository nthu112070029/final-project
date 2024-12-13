#include "Net.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include "shapes/Rectangle.h"
#include "shapes/Point.h"
#include <algorithm>
#include <allegro5/bitmap_draw.h>
#include <iostream>


bool Net::show = false;

 void Net::init() {
   state=Netstate::up;
    
  ImageCenter *IC = ImageCenter::get_instance();
  bitmap = IC->get("./assets/image/Hook/origin.png");
	DataCenter *DC = DataCenter::get_instance();
	 shape.reset(new Rectangle{DC->window_width / 2,
    150,
    DC->window_width / 2 - al_get_bitmap_width(bitmap),
    DC->window_height*0 + al_get_bitmap_height(bitmap)});


}
void
Net::update() {
 DataCenter *DC = DataCenter::get_instance();

   
   switch(state)
   {
           
            case Netstate::up:
              if (DC->key_state[ALLEGRO_KEY_LEFT])
             {
                shape->update_center_x(shape->center_x() - speed);
                state = Netstate::up;
                break;
            }
            else if (DC->key_state[ALLEGRO_KEY_RIGHT])
            {
                shape->update_center_x(shape->center_x() + speed);
                state = Netstate::up;
                break;
            }
            else if(DC->key_state[ALLEGRO_KEY_DOWN])
            {
                state=Netstate::down;
                break;
            }
             if(shape->center_y()<DC->window_height/7){
                vy=100;//出水面碰到人鉤子轉向
                state = Netstate::up;
               
            }  
           
            
            break;
             case Netstate::down:
            {
                 
                static int run=0;
	             double dy = vy / DC->FPS;
               
               
            if(shape->center_y()>DC->window_height-50 )
            {       
                   
                    vy = -100;
                    dy = vy / DC->FPS;
                    run=1;
                    reachbottom=true;
                                 
            }
             if(shape->center_y()<150 && run){
               
                    vy=100;
                    run=0;
                    state = Netstate::up;
                    reachbottom=false;
                    
                    show=false;
                    break;   
            }   
	                shape->update_center_y(shape->center_y() + dy);
                    return; 
                    
            }
        
   }
}

void
Net::draw() {
    ImageCenter *IC = ImageCenter::get_instance();
 
  if(!show)
  {
  bitmap = IC->get("assets/image/Net/invisible.png");
  }
  else if(reachbottom)
    {
         bitmap = IC->get("assets/image/Net/fishcaught.png");
    }
  else {
     bitmap = IC->get("assets/image/Net/show.png");
        
    }
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
        
       
}