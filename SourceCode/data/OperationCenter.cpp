#include "OperationCenter.h"
#include "DataCenter.h"
#include "../monsters/Monster.h"
#include "../fish/Fish.h"
#include "../towers/Tower.h"
#include "../towers/Bullet.h"
#include "../Player.h"
#include "../fishingRod/Hook.h"
#include "../fishingRod/Rod.h"
#include <iostream>
void OperationCenter::update() {
	// Update monsters.
	_update_monster();
	_update_fish();
	// Update towers.
	_update_tower();
	// Update tower bullets.
	_update_towerBullet();
	// If any bullet overlaps with any monster, we delete the bullet, reduce the HP of the monster, and delete the monster if necessary.
	_update_monster_towerBullet();
	// If any monster reaches the end, hurt the player and delete the monster.
	_update_monster_player();
	_update_fish_rodHook();
	
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

void OperationCenter::_update_monster_towerBullet() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	std::vector<Bullet*> &towerBullets = DC->towerBullets;
	for(size_t i = 0; i < monsters.size(); ++i) {
		for(size_t j = 0; j < towerBullets.size(); ++j) {
			// Check if the bullet overlaps with the monster.
			if(monsters[i]->shape->overlap(*(towerBullets[j]->shape))) {
				// Reduce the HP of the monster. Delete the bullet.
				monsters[i]->HP -= towerBullets[j]->get_dmg();
				towerBullets.erase(towerBullets.begin()+j);
				--j;
			}
		}
	}
}
void OperationCenter::_update_fish_rodHook() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Fish*> &fishs = DC->fishs;
	Rod *rod = DC->rod;
	for(size_t i = 0; i < fishs.size(); ++i){
		if(fishs[i]->shape->overlap(*(DC->hook->shape))&&Hook::fishcaught==false){
			
			fishs[i]->HP-=rod->damage;
			if(fishs[i]->HP<=0){
				Hook::fishcaught = true;
				Player *&player = DC->player;
				player->coin +=fishs[i]->get_money();//要解決不erase的情況下 每overlap一次就算damage跟get_money會重複算很多次
				fishs.erase(fishs.begin()+i);
				--i;
			}
			break;
		}
	}
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
	_draw_tower();
	_draw_towerBullet();
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
