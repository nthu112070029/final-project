#include "OperationCenter.h"
#include "DataCenter.h"
#include "FontCenter.h"
#include "../monsters/Monster.h"
#include "../fish/Fish.h"
#include "../Player.h"
#include "../fishingRod/Hook.h"
#include "../fishingRod/Rod.h"
#include"../Net.h"
#include <iostream>
#include<allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
void OperationCenter::update() {
	// Update monsters.
	_update_monster();
	_update_fish();
	// Update towers.
	// If any monster reaches the end, hurt the player and delete the monster.
	_update_monster_player();
	_update_fish_Net();
	_update_fish_Hook() ;
	_update_rod_Hook() ;
	
}

void OperationCenter::_update_monster() {
	std::vector<Monster*> &monsters = DataCenter::get_instance()->monsters;
	for(Monster *monster : monsters)
		monster->update();
}
void OperationCenter::_update_fish() {
	std::vector<Fish*> &fishs = DataCenter::get_instance()->fishs;
	for(Fish *fish: fishs)
		{fish->update();}
		for(size_t i = 0; i < fishs.size(); ++i) {
		
		// Check if the fish reaches the end.
		if(fishs[i]->get_path().empty()) {
			fishs.erase(fishs.begin()+i);
			--i;
		}
	}
}
void OperationCenter:: _update_fish_Net(){

DataCenter *DC = DataCenter::get_instance();
FontCenter *FC = FontCenter::get_instance();
std::vector<Fish*> &fishs = DC->fishs;
	Net* net= DC->net;
	
		for(size_t i = 0; i < fishs.size(); ++i){
		if(fishs[i]->shape->overlap(*(net->shape))){
				Player *&player = DC->player;
				player->coin +=fishs[i]->get_money();			
				fishs.erase(fishs.begin()+i);
				--i;
			break;
		}
	}


}
void OperationCenter::_update_fish_Hook() {
DataCenter *DC = DataCenter::get_instance();
FontCenter *FC = FontCenter::get_instance();
std::vector<Fish*> &fishs = DC->fishs;
	Hook*hook = DC->hook;
	Rod*rod = DC->rod;
		for(size_t i = 0; i < fishs.size(); ++i){
		if(fishs[i]->shape->overlap(*(hook->shape))&&Hook::fishcaught==false){
			if(fishs[i]->get_type()==FishType::Shark&&rod->get_type()==RodType::oringin){
				al_draw_text(
					FC->caviar_dreams[FontSize::LARGE], al_map_rgb(255, 255, 255),
					DC->window_width/2-100., DC->window_height*0+100.,
					ALLEGRO_ALIGN_CENTRE, "M I S S");
					al_flip_display();
					break;
				}
				Hook::fishcaught = true;
				Player *&player = DC->player;
				player->coin +=fishs[i]->get_money();			
				fishs.erase(fishs.begin()+i);
			
				--i;
			break;
		}
	}

}
void OperationCenter::_update_rod_Hook() {
	DataCenter *DC = DataCenter::get_instance();
		Hook*hook = DC->hook;
		Rod *rod=DC->rod;
		static Rodstate pre;
		if(hook->state==Hookstate::up)
		{
			rod->state=Rodstate::up;
				if(rod->type==RodType::plus && pre==Rodstate::down){

				rod->usetimes--;

			}
		}
	pre=rod->state;
}

void OperationCenter::_update_monster_player() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	Player *&player = DC->player;
	for(size_t i = 0; i < monsters.size(); ++i) {
		// Check if the monster is killed.
		if(monsters[i]->HP <= 0) {
			// Monster gets killed. Player receives money.
			player->coin += monsters[i]->get_money();
			monsters.erase(monsters.begin()+i);
			--i;
			// Since the current monsster is killed, we can directly proceed to next monster.
			break;
		}
		// Check if the monster reaches the end.
		if(monsters[i]->get_path().empty()) {
			monsters.erase(monsters.begin()+i);
			--i;
		}
	}
}

void OperationCenter::draw() {
	_draw_monster();
	_draw_fish();
	 _draw_rod_hook();
}

void OperationCenter::_draw_monster() {
	std::vector<Monster*> &monsters = DataCenter::get_instance()->monsters;
	for(Monster *monster : monsters)
		monster->draw();
}
void OperationCenter::_draw_fish() {
	std::vector<Fish*> &fishs = DataCenter::get_instance()->fishs;
	for(Fish *fish : fishs)
		fish->draw();
}




void OperationCenter::_draw_rod_hook(){

	DataCenter *DC = DataCenter::get_instance();
	double x1=DC->hook->x;
	double y1=DC->hook->y;
	double x2=DC->rod->x;
	double y2=DC->rod->y;
	al_draw_line(x1,y1,x2,y2 ,al_map_rgb(0, 0, 0) ,3);
}