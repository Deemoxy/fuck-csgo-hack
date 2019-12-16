#pragma once

#include "vector3d.h"

class CUserCmd {
public:
	char _pad0[ 0x4 ];
	int command_number;
	int tick_count;
	vector3d_t view_angles;
	vector3d_t aim_direction;
	float forward_move;
	float side_move;
	float up_move;
	int buttons;
	char impulse;
	int weapon_select;
	int weapon_sub_type;
	int random_seed;
	short moused_x;
	short moused_y;
	bool has_been_predicted;
};