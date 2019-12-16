#pragma once

#include "vmt.h"
#include "entity.h"

namespace n_hooks {
	extern bool initialization_success;

	extern std::unique_ptr< c_vmt > client_mode;
	extern std::unique_ptr< c_vmt > engine_vgui;

	bool initialize( void );

	namespace n_functions {
		bool __fastcall CreateMove( std::uintptr_t ecx, std::uintptr_t edx, float input_sample_time, CUserCmd* cmd );
		float __fastcall GetViewModelFOV( std::uintptr_t ecx, std::uintptr_t edx );
		void __fastcall Paint( std::uintptr_t ecx, std::uintptr_t edx, int mode );
		bool __fastcall SetupBones( std::uintptr_t ecx, std::uintptr_t edx, void* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime );
	}

	bool unload( void );
}