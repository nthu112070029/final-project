#include "UI.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include <algorithm>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "shapes/Point.h"
#include "shapes/Rectangle.h"
#include "Player.h"
#include "towers/Tower.h"
#include "Level.h"
#include "fishingRod/Rod.h"
#include "fish/Fish.h"
#include "Net.h"

// fixed settings
constexpr char love_img_path[] = "./assets/image/love.png";
constexpr int love_img_padding = 5;
constexpr int tower_img_left_padding = 30;
constexpr int tower_img_top_padding = 30;

void
UI::init() {
	DataCenter *DC = DataCenter::get_instance();
	ImageCenter *IC = ImageCenter::get_instance();

	int tl_x = DC->game_field_length + tower_img_left_padding;
	int tl_y = tower_img_top_padding;
	int max_height = 0;
	// arrange tower shop
	for(size_t i = 0; i < (size_t)(TowerType::TOWERTYPE_MAX); ++i) {
		ALLEGRO_BITMAP *bitmap = IC->get(TowerSetting::tower_menu_img_path[i]);
		int w = al_get_bitmap_width(bitmap);
		int h = al_get_bitmap_height(bitmap);
		if(tl_x + w > DC->window_width) {
			tl_x = DC->game_field_length + tower_img_left_padding;
			tl_y += max_height + tower_img_top_padding;
			max_height = 0;
		}
		tower_items.emplace_back(bitmap, Point{tl_x, tl_y}, TowerSetting::tower_price[i]);
		tl_x += w + tower_img_left_padding;
		max_height = std::max(max_height, h);
	}
	debug_log("<UI> state: change to HALT\n");
	state = STATE::HALT;
	on_item = -1;
}

void
UI::update() {
	DataCenter *DC = DataCenter::get_instance();
	const Point &mouse = DC->mouse;

	switch(state) {
		case STATE::HALT: {
			for(size_t i = 0; i < tower_items.size(); ++i) {
				auto &[bitmap, p, price] = tower_items[i];
				int w = al_get_bitmap_width(bitmap);
				int h = al_get_bitmap_height(bitmap);
				// hover on a shop tower item
				if(mouse.overlap(Rectangle{p.x, p.y, p.x+w, p.y+h})) {
					on_item = i;
					debug_log("<UI> state: change to HOVER\n");
					state = STATE::HOVER;
					break;
				}
			}
			break;
		} case STATE::HOVER: {
			auto &[bitmap, p, price] = tower_items[on_item];
			int w = al_get_bitmap_width(bitmap);
			int h = al_get_bitmap_height(bitmap);
			if(!mouse.overlap(Rectangle{p.x, p.y, p.x+w, p.y+h})) {
				on_item = -1;
				debug_log("<UI> state: change to HALT\n");
				state = STATE::HALT;
				break;
			}
			// click mouse left button
			if(DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
				// no money
				if(price > DC->player->coin) {
					debug_log("<UI> Not enough money to buy tower %d.\n", on_item);
					break;
				}
				debug_log("<UI> state: change to SELECT\n");
				state = STATE::SELECT;
			}
			break;
		} case STATE::SELECT: {
			// click mouse left button: place
			if(DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
				debug_log("<UI> state: change to PLACE\n");
				state = STATE::PLACE;
			}
			// click mouse right button: cancel
			if(DC->mouse_state[2] && !DC->prev_mouse_state[2]) {
				on_item = -1;
				debug_log("<UI> state: change to HALT\n");
				state = STATE::HALT;
			}
			break;
		} case STATE::PLACE: {
			// update rod
			DC->player->coin -=300;
			DC->rod->usetimes+=10;
			DC->rod->type=RodType::plus;

			//update coldtime effect
			DC->player->coin -=300;
			for(Fish *f : DC->fishs) {
				f->set_v(0);
			}

			//update net
			DC->player->coin -=50;
			DC->net->show=true;
			
			

			
			
			debug_log("<UI> state: change to HALT\n");
			state = STATE::HALT;
			break;
		}
	}
}

void
UI::draw() {
	DataCenter *DC = DataCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();
	const Point &mouse = DC->mouse;
	// draw HP
	const int &game_field_length = DC->game_field_length;
	// draw coin
	const int &player_coin = DC->player->coin;
	al_draw_textf(
		FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
		600,0,
		ALLEGRO_ALIGN_LEFT, "coin: %5d", player_coin);
		const int &player_countdown = DC->player->countdown;
		//draw countdown
	al_draw_textf(
		FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
		0,0,
		ALLEGRO_ALIGN_LEFT, "countdown:%4d", player_countdown);
		const int &player_goal = DC->player->goal;
	//draw goal
	al_draw_textf(
		FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
		300,0,
		ALLEGRO_ALIGN_LEFT, "goal:%4d coin", player_goal);
	// draw tower shop items
	for(auto &[bitmap, p, price] : tower_items) {
		int w = al_get_bitmap_width(bitmap);
		int h = al_get_bitmap_height(bitmap);
		al_draw_bitmap(bitmap, p.x, p.y, 0);
		al_draw_rectangle(
			p.x - 1, p.y - 1,
			p.x + w + 1, p.y + h + 1,
			al_map_rgb(0, 0, 0), 1);
		al_draw_textf(
			FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
			p.x + w / 2, p.y + h,
			ALLEGRO_ALIGN_CENTRE, "%d", price);
	}

	switch(state) {
		static Tower *selected_tower = nullptr;
		case STATE::HALT: {
			// No tower should be selected for HALT state.
			if(selected_tower != nullptr) {
				delete selected_tower;
				selected_tower = nullptr;
			}
			break;
		} case STATE::HOVER: {
			auto &[bitmap, p, price] = tower_items[on_item];
			int w = al_get_bitmap_width(bitmap);
			int h = al_get_bitmap_height(bitmap);
			// Create a semitransparent mask covered on the hovered tower.
			al_draw_filled_rectangle(p.x, p.y, p.x + w, p.y + h, al_map_rgba(50, 50, 50, 64));
			break;
		}
		case STATE::SELECT: {
			// If a tower is selected, we new a corresponding tower for previewing purpose.
		
		}
		case STATE::PLACE: {
			
		}
	}
}
