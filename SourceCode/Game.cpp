#include "Game.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/OperationCenter.h"
#include "data/SoundCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include "Player.h"
#include "Level.h"
#include "hero.h"
#include "Net.h"
#include "fishingRod/Hook.h"
#include "fishingRod/Rod.h"
//add hare
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <cstring>
#include "shapes/Rectangle.h"
#include <iostream>
#include "fish/Fish.h"

// fixed settings
constexpr char game_icon_img_path[] = "./assets/image/game_icon.png";
constexpr char game_start_sound_path[] = "./assets/sound/growl.wav";
constexpr char background_img_path[] = "./assets/image/StartBackground.jpg";
constexpr char background_sound_path[] = "./assets/sound/BackgroundMusic.ogg";
constexpr char background_img2_path[] = "./assets/image/StartBackground2.jpg";
/**
 * @brief Game entry.
 * @details The function processes all allegro events and update the event state to a generic data storage (i.e. DataCenter).
 * For timer event, the game_update and game_draw function will be called if and only if the current is timer.
 */
void
Game::execute() {
	DataCenter *DC = DataCenter::get_instance();
	// main game loop
	bool run = true;
	while(run) {
		// process all events here
		al_wait_for_event(event_queue, &event);
		switch(event.type) {
			case ALLEGRO_EVENT_TIMER: {
				run &= game_update();
				game_draw();
				break;
			} case ALLEGRO_EVENT_DISPLAY_CLOSE: { // stop game
				run = false;
				break;
			} case ALLEGRO_EVENT_KEY_DOWN: {
				DC->key_state[event.keyboard.keycode] = true;
				break;
			} case ALLEGRO_EVENT_KEY_UP: {
				DC->key_state[event.keyboard.keycode] = false;
				break;
			} case ALLEGRO_EVENT_MOUSE_AXES: {
				DC->mouse.x = event.mouse.x;
				DC->mouse.y = event.mouse.y;
				break;
			} case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
				DC->mouse_state[event.mouse.button] = true;
				break;
			} case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
				DC->mouse_state[event.mouse.button] = false;
				break;
			} default: break;
		}
	}
}

/**
 * @brief Initialize all allegro addons and the game body.
 * @details Only one timer is created since a game and all its data should be processed synchronously.
 */
Game::Game() {
	DataCenter *DC = DataCenter::get_instance();
	GAME_ASSERT(al_init(), "failed to initialize allegro.");

	// initialize allegro addons
	bool addon_init = true;
	addon_init &= al_init_primitives_addon();
	addon_init &= al_init_font_addon();
	addon_init &= al_init_ttf_addon();
	addon_init &= al_init_image_addon();
	addon_init &= al_init_acodec_addon();
	GAME_ASSERT(addon_init, "failed to initialize allegro addons.");

	// initialize events
	bool event_init = true;
	event_init &= al_install_keyboard();
	event_init &= al_install_mouse();
	event_init &= al_install_audio();
	GAME_ASSERT(event_init, "failed to initialize allegro events.");

	// initialize game body
	GAME_ASSERT(
		display = al_create_display(DC->window_width, DC->window_height),
		"failed to create display.");
	GAME_ASSERT(
		timer = al_create_timer(1.0 / DC->FPS),
		"failed to create timer.");
	GAME_ASSERT(
		event_queue = al_create_event_queue(),
		"failed to create event queue.");

	debug_log("Game initialized.\n");
	game_init();
}

/**
 * @brief Initialize all auxiliary resources.
 */
void
Game::game_init() {
	DataCenter *DC = DataCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	ImageCenter *IC = ImageCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();
	// set window icon
	game_icon = IC->get(game_icon_img_path);
	al_set_display_icon(display, game_icon);

	// register events to event_queue
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// init sound setting
	SC->init();

	// init font setting
	FC->init();

	ui = new UI();
	ui->init();

	DC->level->init();
	//DC->hero->init();
	DC->rod->init();
	DC->hook->init();
	DC->net->init();
//add here
	// game start
	background = IC->get(background_img_path);
	background2 = IC->get(background_img2_path);
	debug_log("Game state: change to START\n");
	state = STATE::INIT;
	al_start_timer(timer);
}

/**
 * @brief The function processes all data update.
 * @details The behavior of the whole game body is determined by its state.
 * @return Whether the game should keep running (true) or reaches the termination criteria (false).
 * @see Game::STATE
 */
bool
Game::game_update() {
	DataCenter *DC = DataCenter::get_instance();
	OperationCenter *OC = OperationCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	static ALLEGRO_SAMPLE_INSTANCE *background = nullptr;
	ALLEGRO_MOUSE_STATE mouse_state;
	const Point &mouse = DC->mouse;
	switch(state) {
		case STATE::INIT : {
			if(DC->key_state[ALLEGRO_KEY_Q]){
				state=STATE::END;
			}
			if(mouse.overlap(Rectangle{443.1,463.11,539.44,511.4})){
				al_get_mouse_state(&mouse_state);
				if(mouse_state.buttons&1)
					state=STATE::END;
			}
		}
		case STATE::START: {
			static bool is_played = false;
			static ALLEGRO_SAMPLE_INSTANCE *instance = nullptr;
			if(!is_played) {
				instance = SC->play(game_start_sound_path, ALLEGRO_PLAYMODE_ONCE);
				DC->level->load_level(1);
				DC->player->countdown=DC->level->time;
				DC->player->goal=DC->level->goal;
				is_played = true;
			}

			if(!SC->is_playing(instance)&&mouse.overlap(Rectangle{73.4,463.11,161.47,511.4}) ){
				//debug_log("<Game> state: change to LEVEL\n");
				al_get_mouse_state(&mouse_state);
				if(mouse_state.buttons&1)
					state=STATE::LEVEL;
			}
			break;
		} case STATE::LEVEL: {
			static bool BGM_played = false;
			if(!BGM_played) {
				background = SC->play(background_sound_path, ALLEGRO_PLAYMODE_LOOP);
				BGM_played = true;
			}

			if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
				SC->toggle_playing(background);
				debug_log("<Game> state: change to PAUSE\n");
				state = STATE::PAUSE;
			}
		
		
			if(DC->player->countdown==0 && DC->player->coin< DC->player->goal){
				debug_log("<Game> state: change to END\n");
				state = STATE::END;
			}
			if(DC->player->countdown==0 && DC->player->coin>=DC->player->goal){
				debug_log("<Game> state: change to Level\n");
				static int level2=0;
				if(!level2){
				DC->level->load_level(2);
				DC->player->countdown=DC->level->time;
				DC->player->goal=DC->level->goal;
				level2=1;
				}
				else{
				DC->level->load_level(3);
				DC->player->countdown=DC->level->time;
				DC->player->goal=DC->level->goal;
				}

			}
			if(mouse.overlap(Rectangle{624.86,41.66,687.65,108.31})){
				if(DC->mouse_state[1] && !DC->prev_mouse_state[1]){
					DC->player->coin -=300;
					DC->rod->usetimes+=4;
					DC->rod->type=RodType::plus;
				}			
			}
			if(mouse.overlap(Rectangle{724.11,41.66,779.16,108.31})){
				if(DC->mouse_state[1] && !DC->prev_mouse_state[1]){
				DC->player->coin -=300;
				for(Fish *f : DC->fishs) {
					f->set_v(0);
				}
				}	
			}
			if(mouse.overlap(Rectangle{624.86,141.66,687.65,218.07})){
				if(DC->mouse_state[1] && !DC->prev_mouse_state[1]){
					DC->player->coin -=50;
					DC->net->show=true;
				}	
			}
			break;
		} case STATE::PAUSE: {
			if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
				SC->toggle_playing(background);
				debug_log("<Game> state: change to LEVEL\n");
				state = STATE::LEVEL;
			}
			if(DC->key_state[ALLEGRO_KEY_Q]){
				state=STATE::END;
			}
			if(mouse.overlap(Rectangle{443.1,463.11,539.44,511.4})){
				al_get_mouse_state(&mouse_state);
				if(mouse_state.buttons&1)
					state=STATE::END;
			}
			if(mouse.overlap(Rectangle{205.5,463.11,390.83,511.4})){
				al_get_mouse_state(&mouse_state);
				if(mouse_state.buttons&1){
					SC->toggle_playing(background);
					state=STATE::LEVEL;}
			}
			break;
		} case STATE::END: {
			return false;
		}
	}
	// If the game is not paused, we should progress update.
	if(state != STATE::PAUSE&&state !=STATE::INIT) {
		DC->player->update();
		SC->update();
		ui->update();
		if(state != STATE::START) {
			DC->level->update();
			//DC->hero->update();
			DC->hook->update();
			DC->rod->update();
			DC->net->update();
			//add here
			OC->update();
		}
	}
	// game_update is finished. The states of current frame will be previous states of the next frame.
	memcpy(DC->prev_key_state, DC->key_state, sizeof(DC->key_state));
	memcpy(DC->prev_mouse_state, DC->mouse_state, sizeof(DC->mouse_state));
	return true;
}

/**
 * @brief Draw the whole game and objects.
 */
void
Game::game_draw() {
	DataCenter *DC = DataCenter::get_instance();
	OperationCenter *OC = OperationCenter::get_instance();
	//FontCenter *FC = FontCenter::get_instance();

	// Flush the screen first.
	al_clear_to_color(al_map_rgb(100, 100, 100));
	if(state == STATE::INIT){
		al_draw_bitmap(background2, 0, 0, 0);
	}
	if(state != STATE::END && state != STATE::INIT) {
		// background
		al_draw_bitmap(background, 0, 0, 0);
		ALLEGRO_BITMAP* image = al_load_bitmap("./assets/image/image.jpg");
    	int image_width = al_get_bitmap_width(image);
    	//int image_height = al_get_bitmap_height(image);	
		int image_x = DC->window_width-image_width;
		int image_y = 20;
		al_draw_bitmap(image, image_x, image_y, 0);
/*		if(DC->game_field_length < DC->window_width)
			al_draw_filled_rectangle(
				DC->game_field_length, 0,
				DC->window_width, DC->window_height,
				al_map_rgb(100, 100, 100));
		if(DC->game_field_length < DC->window_height)
			al_draw_filled_rectangle(
				0, DC->game_field_length,
				DC->window_width, DC->window_height,
				al_map_rgb(100, 
				100, 100));*/
		/*	al_draw_filled_rectangle(
				DC->game_field_length, 0,
				DC->window_width, DC->window_height*0+170,
				al_map_rgb(255, 255, 255));*/
		// user interface
		if(state != STATE::START) {
			DC->level->draw();
			//DC->hero->draw();
			DC->rod->draw();
			DC->hook->draw();
			DC->net->draw();
			// add here
			ui->draw();
			OC->draw();
		}
	}
	switch(state) {
		case STATE::INIT:{
		}
		case STATE::START: {
		} case STATE::LEVEL: {
			break;
		} case STATE::PAUSE: {
			// game layout cover
			al_draw_filled_rectangle(0, 0, DC->window_width, DC->window_height, al_map_rgba(50, 50, 50, 64));
			al_draw_bitmap(background2, 0, 0, 0);
			break;
		} case STATE::END: {
		}
	}
	al_flip_display();
}

Game::~Game() {
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
}
