#include "OperationCenter.h"
#include "DataCenter.h"
#include "FontCenter.h"
#include "../fish/Fish.h"
#include "../towers/Tower.h"
#include "../towers/Bullet.h"
#include "../Player.h"
#include "../fishingRod/Hook.h"
#include "../fishingRod/Rod.h"
#include <iostream>
#include<allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
void OperationCenter::update() {
	
	_update_fish();
	// Update towers.
	_update_tower();
	// Update tower bullets.
	_update_towerBullet();
	// If any bullet overlaps with any monster, we delete the bullet, reduce the HP of the monster, and delete the monster if necessary.
	

	_update_fish_Hook() ;
	_update_rod_Hook() ;
	
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

void OperationCenter::_update_tower() {
	std::vector<Tower*> &towers = DataCenter::get_instance()->towers;
	for(Tower *tower : towers)
		tower->update();
}

void OperationCenter::_update_towerBullet() {
	std::vector<Bullet*> &towerBullets = DataCenter::get_instance()->towerBullets;
	for(Bullet *towerBullet : towerBullets)
		towerBullet->update();
	// Detect if a bullet flies too far (exceeds its fly distance limit), which means the bullet lifecycle has ended.
	for(size_t i = 0; i < towerBullets.size(); ++i) {
		if(towerBullets[i]->get_fly_dist() <= 0) {
			towerBullets.erase(towerBullets.begin()+i);
			--i;
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
		if(hook->state==Hookstate::up)
		{
			rod->state=Rodstate::up;
			if(rod->type==RodType::plus)rod->usetimes--;
		}
		

}



void OperationCenter::draw() {

	_draw_fish();
	_draw_tower();
	_draw_towerBullet();
	 _draw_rod_hook();
}


void OperationCenter::_draw_fish() {
	std::vector<Fish*> &fishs = DataCenter::get_instance()->fishs;
	for(Fish *fish : fishs)
		fish->draw();
}

void OperationCenter::_draw_tower() {
	std::vector<Tower*> &towers = DataCenter::get_instance()->towers;
	for(Tower *tower : towers)
		tower->draw();
}

void OperationCenter::_draw_towerBullet() {
	std::vector<Bullet*> &towerBullets = DataCenter::get_instance()->towerBullets;
	for(Bullet *towerBullet : towerBullets)
		towerBullet->draw();
}
void OperationCenter::_draw_rod_hook(){

	DataCenter *DC = DataCenter::get_instance();
	double x1=DC->hook->x;
	double y1=DC->hook->y;
	double x2=DC->rod->x;
	double y2=DC->rod->y;
	al_draw_line(x1,y1,x2,y2 ,al_map_rgb(0, 0, 0) ,3);
	
}