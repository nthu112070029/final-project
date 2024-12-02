#include "Fish.h"
#include "salmon.h"
#include "snapper.h"
#include "tuna.h"
#include "shark.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Level.h"
#include "../shapes/Point.h"
#include "../shapes/Rectangle.h"
#include "../Utils.h"
#include <allegro5/allegro_primitives.h>

using namespace std;

// fixed settings
enum class Fishstate {
	swim,up
};
namespace FishSetting {
	static constexpr char Fish_imgs_root_path[static_cast<int>(FishType::FishTYPE_MAX)][40] = {
		"./assets/image/fish/Salmon",
		"./assets/image/fish/Snapper",
		"./assets/image/fish/Tuna",
		"./assets/image/fish/Shark"
	};
	static constexpr char state_path_prefix[][10] = {
		"swim","up"
	};
}


Fish *Fish::create_Fish(FishType type, const vector<Point> &path) {
	switch(type) {
		case FishType::Salmon: {
			return new Salmon{path};
		}
		case FishType::Snapper: {
			return new Snapper{path};
		}
		case FishType::Tuna: {
			return new Tuna{path};
		}
		case FishType::Shark: {
			return new Shark{path};
		}
		case FishType::FishTYPE_MAX: {}
	}
	GAME_ASSERT(false, "Fish type error.");
}




Fish::Fish(const vector<Point> &path, FishType type) {
	DataCenter *DC = DataCenter::get_instance();

	shape.reset(new Rectangle{0, 0, 0, 0});
	this->type = type;
	state= Fishstate::swim;
	bitmap_img_id = 0;
	bitmap_switch_counter = 0;
	for(const Point &p : path)
		this->path.push(p);
	if(!path.empty()) {
		const Point &grid = this->path.front();
		const Rectangle &region = DC->level->grid_to_region(grid);
		// Temporarily set the bounding box to the center (no area) since we haven't got the hit box of the Fish.
		shape.reset(new Rectangle{region.center_x(), region.center_y(), region.center_x(), region.center_y()});
		this->path.pop();
	}
}

void
Fish::update() {
	DataCenter *DC = DataCenter::get_instance();
	ImageCenter *IC = ImageCenter::get_instance();

	// After a period, the bitmap for this monster should switch from (i)-th image to (i+1)-th image to represent animation.
	if(bitmap_switch_counter) --bitmap_switch_counter;
	else {
		bitmap_img_id = (bitmap_img_id + 1) % (bitmap_img_ids[static_cast<int>(state)].size());
		bitmap_switch_counter = bitmap_switch_freq;
	}
	// v (velocity) divided by FPS is the actual moving pixels per frame.
	double movement = v / DC->FPS;
	// Keep trying to move to next destination in "path" while "path" is not empty and we can still move.
	while(!path.empty() && movement > 0) {
		const Point &grid = this->path.front();
		const Rectangle &region = DC->level->grid_to_region(grid);
		const Point &next_goal = Point{region.center_x(), region.center_y()};

		// Extract the next destination as "next_goal". If we want to reach next_goal, we need to move "d" pixels.
		double d = Point::dist(Point{shape->center_x(), shape->center_y()}, next_goal);
		
		if(d < movement) {
			// If we can move more than "d" pixels in this frame, we can directly move onto next_goal and reduce "movement" by "d".
			movement -= d;
			
			shape.reset(new Rectangle{
				next_goal.x, next_goal.y,
				next_goal.x, next_goal.y
		});
			path.pop();
		} else {
			// Otherwise, we move exactly "movement" pixels.
			double dx = (next_goal.x - shape->center_x()) / d * movement;
			double dy = (next_goal.y - shape->center_y()) / d * movement;
			
			shape->update_center_x(shape->center_x() + dx);
			shape->update_center_y(shape->center_y() + dy);
			movement = 0;
		}
		
		
	}
	// Update real hit box for monster.
	char buffer[50];
	sprintf(
		buffer, "%s/%s_%d.png",
		FishSetting::Fish_imgs_root_path[static_cast<int>(type)],
		FishSetting::state_path_prefix[static_cast<int>(state)],
		bitmap_img_ids[static_cast<int>(state)][bitmap_img_id]);
	ALLEGRO_BITMAP *bitmap = IC->get(buffer);
	const double &cx = shape->center_x();
	const double &cy = shape->center_y();
	// We set the hit box slightly smaller than the actual bounding box of the image because there are mostly empty spaces near the edge of a image.
	const int &h = al_get_bitmap_width(bitmap) * 0.8;
	const int &w = al_get_bitmap_height(bitmap) * 0.8;
	shape.reset(new Rectangle{
		(cx - w / 2.), (cy - h / 2.),
		(cx - w / 2. + w), (cy - h / 2. + h)
	});

}

void
Fish::draw() {
	ImageCenter *IC = ImageCenter::get_instance();
	char buffer[50];
	sprintf(
		buffer, "%s/%s_%d.png",
		FishSetting::Fish_imgs_root_path[static_cast<int>(type)],
		FishSetting::state_path_prefix[static_cast<int>(state)],
		bitmap_img_ids[static_cast<int>(state)][bitmap_img_id]);
	
	ALLEGRO_BITMAP *bitmap = IC->get(buffer);
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}
