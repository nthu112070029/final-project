#ifndef OPERATIONCENTER_H_INCLUDED
#define OPERATIONCENTER_H_INCLUDED

/**
 * @brief Class that defines functions for all object operations.
 * @details Object self-update, draw, and object-to-object interact functions are defined here.
 */
class OperationCenter
{
public:
	static OperationCenter *get_instance() {
		static OperationCenter OC;
		return &OC;
	}
	/**
	 * @brief Highest level update function.
	 * @details Calls all other update functions.
	 */
	void update();
	/**
	 * @brief Highest level draw function.
	 * @details Calls all other draw functions.
	 */
	void draw();
private:
	OperationCenter() {}
private:
	void _update_monster();
	void _update_fish();
	void _update_monster_player();
	void _update_rod_Hook();
	void _update_fish_Net();
	void _update_fish_Hook();
	
private:
	void _draw_monster();
	void _draw_fish();
	void _draw_rod_hook();
};

#endif