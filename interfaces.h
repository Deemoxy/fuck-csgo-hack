#pragma once

#include "sdk.h"

namespace n_interfaces {
	extern bool initialization_success;

	extern IBaseClientDLL* client;
	extern IClientMode* client_mode;
	extern IVEngineClient* engine;
	extern IClientEntityList* entity_list;
	extern IVEngineVGui* engine_vgui;
	extern ISurface* surface;
	extern CGlobalVarsBase* global_vars_base;
	extern IPrediction* prediction;
	extern IMoveHelper* move_helper;
	extern IGameMovement* game_movement;
	extern IWeaponSystem* weapon_system;

	bool initialize( void );
}