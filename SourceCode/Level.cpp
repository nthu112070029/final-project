#include "Level.h"
#include <string>
#include "Utils.h"
#include "monsters/Monster.h"
#include "fish/Fish.h"
#include "data/DataCenter.h"
#include <allegro5/allegro_primitives.h>
#include "shapes/Point.h"
#include "shapes/Rectangle.h"
#include <array>

using namespace std;

// fixed settings
namespace LevelSetting {
	constexpr char level_path_format[] = "./assets/level/LEVEL%d.txt";
	//! @brief Grid size for each level.
	constexpr array<int, 4> grid_size = {
		85,85,85,85
	};
	constexpr int monster_spawn_rate = 200;
	constexpr int monster_spawn_rate1 = 300;
	constexpr int monster_spawn_rate2 = 500;
	constexpr int monster_spawn_rate3 = 600;
	constexpr int monster_spawn_rate4 = 200;
};

void
Level::init() {
	level = -1;
	grid_w = -1;
	grid_h = -1;
	monster_spawn_counter = 0;
	monster_spawn_counter1 = 0;
	monster_spawn_counter2 = 0;
	monster_spawn_counter3 = 0;
	monster_spawn_counter4 = 0;
}

/**
 * @brief Loads level data from input file. The input file is required to follow the format.
 * @param lvl level index. The path format is a fixed setting in code.
 * @details The content of the input file should be formatted as follows:
 *          * Total number of monsters.
 *          * Number of each different number of monsters. The order and number follows the definition of MonsterType.
 *          * Indefinite number of Point (x, y), represented in grid format.
 * @see level_path_format
 * @see MonsterType
 */
void
Level::load_level(int lvl) {
	DataCenter *DC = DataCenter::get_instance();

	char buffer[50];
	sprintf(buffer, LevelSetting::level_path_format, lvl);
	FILE *f = fopen(buffer, "r");
	GAME_ASSERT(f != nullptr, "cannot find level.");
	level = lvl;
	grid_w = DC->game_field_length / LevelSetting::grid_size[lvl];
	grid_h = DC->game_field_length / LevelSetting::grid_size[lvl];
	num_of_monsters.clear();
	road_path.clear();
	road_path1.clear();
	road_path2.clear();
	road_path3.clear();
	road_path4.clear();

	int num;
	// load time
	fscanf(f, "%d", &num);
	time=num;
	fscanf(f, "%d", &num);
	goal=num;
	// read total number of monsters & number of each monsters
	for(size_t i = 0; i < static_cast<size_t>(MonsterType::MONSTERTYPE_MAX); ++i) {
		fscanf(f, "%d", &num);
		num_of_monsters.emplace_back(num);
	}

	// read road path
	for(size_t i = 0; i < 7; ++i) {
		fscanf(f, "%d", &num);
		int w = num % grid_w;
		int h = num / grid_h;
		road_path.emplace_back(w, h);
	}
	for(size_t i = 0; i < 7; ++i) {
		fscanf(f, "%d", &num);
		int w = num % grid_w;
		int h = num / grid_h;
		road_path1.emplace_back(w, h);
	}
	
	for(size_t i = 0; i < 7; ++i) {
		fscanf(f, "%d", &num);
		int w = num % grid_w;
		int h = num / grid_h;
		road_path2.emplace_back(w, h);
	}
	for(size_t i = 0; i < 7; ++i) {
		fscanf(f, "%d", &num);
		int w = num % grid_w;
		int h = num / grid_h;
		road_path3.emplace_back(w, h);
	}
	for(size_t i = 0; i < 7; ++i) {
		fscanf(f, "%d", &num);
		int w = num % grid_w;
		int h = num / grid_h;
		road_path4.emplace_back(w, h);
	}
	
	debug_log("<Level> load level %d.\n", lvl);
}

/**
 * @brief Updates monster_spawn_counter and create monster if needed.
*/
void
Level::update() {
	if(monster_spawn_counter && monster_spawn_counter1 && monster_spawn_counter2&& monster_spawn_counter3 ) {
		monster_spawn_counter--;
		monster_spawn_counter1--;
		monster_spawn_counter2--;
		monster_spawn_counter3--;
		//monster_spawn_counter4--;
		return;
	}
	DataCenter *DC = DataCenter::get_instance();

	if(monster_spawn_counter<=0)
	{
		if(num_of_monsters[0] > 0) 
		{
			DC->fishs.emplace_back(Fish::create_Fish(static_cast<FishType>(1), DC->level->get_road_path()));
			num_of_monsters[0]--;
		}

		monster_spawn_counter = LevelSetting::monster_spawn_rate;
		monster_spawn_counter1--;
		monster_spawn_counter2--;
		monster_spawn_counter3--;
		//monster_spawn_counter4--;
	}
	 if(monster_spawn_counter1<=0)
	{
		if(num_of_monsters[1] > 0) 
		{
			DC->fishs.emplace_back(Fish::create_Fish(static_cast<FishType>(0), DC->level->get_road_path1()));
			num_of_monsters[1]--;
		}

		monster_spawn_counter1 = LevelSetting::monster_spawn_rate1;
		monster_spawn_counter--;
		monster_spawn_counter2--;
		monster_spawn_counter3--;
		//monster_spawn_counter4--;
	}
	 if(monster_spawn_counter2<=0)
	{
		if(num_of_monsters[2] > 0) 
		{
			DC->fishs.emplace_back(Fish::create_Fish(static_cast<FishType>(2), DC->level->get_road_path2()));
			num_of_monsters[2]--;
		}
		monster_spawn_counter2 = LevelSetting::monster_spawn_rate2;
		monster_spawn_counter1--;
		monster_spawn_counter--;
		monster_spawn_counter3--;
		//monster_spawn_counter4--;
	}
	 if(monster_spawn_counter3<=0)
	{
		if(num_of_monsters[3] > 0) 
		{
				DC->fishs.emplace_back(Fish::create_Fish(static_cast<FishType>(3), DC->level->get_road_path3()));
			num_of_monsters[3]--;
		}
		monster_spawn_counter3 = LevelSetting::monster_spawn_rate3;
		monster_spawn_counter1--;
		monster_spawn_counter2--;
		monster_spawn_counter--;
		//monster_spawn_counter4--;
	}
	


}

void
Level::draw() {
	DataCenter *DC = DataCenter::get_instance();
	if(level == -1) return;
	for(auto &[i, j] : road_path) {
		int x1 = i * LevelSetting::grid_size[level];
		int y1 = j * LevelSetting::grid_size[level];
		int x2 = x1 + LevelSetting::grid_size[level];
		int y2 = y1 + LevelSetting::grid_size[level];
		al_draw_filled_rectangle(x1, y1,x2 , y2, al_map_rgb(0,0,255));
	}
	for(auto &[i, j] : road_path1) {
		int x1 = i * LevelSetting::grid_size[level];
		int y1 = j * LevelSetting::grid_size[level];
		int x2 = x1 + LevelSetting::grid_size[level];
		int y2 = y1 + LevelSetting::grid_size[level];
		al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0,0,255));
	}
	for(auto &[i, j] : road_path2) {
		int x1 = i * LevelSetting::grid_size[level];
		int y1 = j * LevelSetting::grid_size[level];
		int x2 = x1 + LevelSetting::grid_size[level];
		int y2 = y1 + LevelSetting::grid_size[level];
		al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0,0,255));
	}
	for(auto &[i, j] : road_path3) {
		int x1 = i * LevelSetting::grid_size[level];
		int y1 = j * LevelSetting::grid_size[level];
		int x2 = x1 + LevelSetting::grid_size[level];
		int y2 = y1 + LevelSetting::grid_size[level];
		al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0,0,255));
	}
	for(auto &[i, j] : road_path4) {
		int x1 = i * LevelSetting::grid_size[level];
		int y1 = j * LevelSetting::grid_size[level];
		int x2 = x1 + LevelSetting::grid_size[level];
		int y2 = y1 + LevelSetting::grid_size[level];
		al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0,0,255));
	}
	
}

bool
Level::is_onroad(const Rectangle &region) {
	for(const Point &grid : road_path) {
		if(grid_to_region(grid).overlap(region))
			return true;
	}
	return false;
}

Rectangle
Level::grid_to_region(const Point &grid) const {
	int x1 = grid.x * LevelSetting::grid_size[level];
	int y1 = grid.y * LevelSetting::grid_size[level];
	int x2 = x1 + LevelSetting::grid_size[level];
	int y2 = y1 + LevelSetting::grid_size[level];
	return Rectangle{x1, y1, x2, y2};
}
