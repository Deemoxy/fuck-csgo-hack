#pragma once

#include "utilities.h"

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/231080-getting-wpndata-vtable-indexes.html

struct CCSWeaponData {
	char _pad0[ 0x4 ]; // 0x0
	char* console_name; // 0x4
	char _pad1[ 0xc ];// 0x8
	int max_clip1; // 0x14
	int max_clip2; // 0x18
	int default_clip1; // 0x1c
	int default_clip2; // 0x20
	int primary_reserve_ammo_max; // 0x24
	int secondary_reserve_ammo_max; // 0x28
	char* world_model; // 0x2c
	char* view_model; // 0x30
	char* dropped_model; // 0x34
	char _pad2[ 0x50 ];// 0x38
	char* hud_name; // 0x88
	char* weapon_name; // 0x8c
};

class IWeaponSystem {
	virtual void unused0( ) = 0;
	virtual void unused1( ) = 0;
public:
	virtual CCSWeaponData* get_weapon_data( unsigned index ) const = 0;
};