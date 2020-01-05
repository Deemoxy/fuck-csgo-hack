#include "hooks.h"

#include "animations.h"

#include <string>

namespace n_hooks {
	bool __fastcall n_functions::SetupBones( std::uintptr_t ecx, std::uintptr_t edx, void* bone_to_world_out, int max_bones, int bone_mask, float current_time ) {

		C_CSPlayer* player = reinterpret_cast < C_CSPlayer* >( std::uintptr_t( ecx ) - 0x4 );

		auto original_fn = g_animations.track[ player->networkable( )->ent_index( ) ].renderable->get_original_function<decltype( &SetupBones )>( 13 );

		if ( player && player->is_alive( ) ) {
			//bone_mask = bone_mask | 0x80000;
			//
			//if ( *(DWORD* )( ecx + 0x28B8 ) != -1 ) {
			//	if ( bone_mask != -1 ) 
			//		bone_mask = *(DWORD* )( ecx + 0x2698 );

			//	/*if ( Interfaces::Cvar->FindVar( "cl_setupallbones" )->GetInt( ) )
			//		bone_mask = bone_mask | 0xFFF00;*/

			//	if ( !*(BYTE* )( ecx + 0x28D ) )
			//		bone_mask = bone_mask | 0xFFF00;

			//	int nLOD = 0;
			//	int nMask = 0x400; // BONE_USED_BY_VERTEX_LOD0
			//	do {
			//		if ( bone_mask & nMask )
			//			break;
			//		++nLOD;
			//		nMask *= 2;
			//	} while ( nLOD < 8 );
			//	if ( nLOD < 8 ) {
			//		int v26 = 8 - nLOD;
			//		do {
			//			bone_mask |= nMask;
			//			nMask *= 2;
			//			--v26;
			//		} while ( v26 );
			//	}

			//	if ( *(BYTE* )( ecx + 0x2EA ) && n_interfaces::prediction->in_prediction( ) )
			//		current_time = n_interfaces::prediction->saved_time;
			//	

			//	if ( *(DWORD* )( ecx + 0x268C ) != g_iModelBoneCounter )
			// }

			// fuck this
		}

		return original_fn( ecx, edx, bone_to_world_out, max_bones, bone_mask, current_time );
	}
}