#include "engine_prediction.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/server/player_command.cpp

namespace n_engine_prediction {
	float old_current_time;
	float old_frame_time;
	void* move_data;
	int* prediction_random_seed;
	int* prediction_player;

	void initialize( C_CSPlayer* player, CUserCmd* cmd ) {
		if ( !move_data )
			move_data = std::malloc( 182 );

		old_current_time = n_interfaces::global_vars_base->current_time;
		old_frame_time = n_interfaces::global_vars_base->frame_time;

		n_interfaces::global_vars_base->current_time = player->get_tick_base( ) * n_interfaces::global_vars_base->interval_per_tick;
		n_interfaces::global_vars_base->frame_time = *reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( n_interfaces::prediction ) + 0x0A ) ? 0 : n_interfaces::global_vars_base->interval_per_tick;

		if ( !prediction_random_seed || !prediction_player ) {
			prediction_random_seed = *reinterpret_cast< int** >( n_utilities::pattern_scan( "client_panorama.dll", "A3 ? ? ? ? 66 0F 6E 86" ) + 0x1 );
			prediction_player = *reinterpret_cast< int** >( n_utilities::pattern_scan( "client_panorama.dll", "89 35 ? ? ? ? F3 0F 10 48" ) + 0x2 );
		}

		// CPlayerMove::StartCommand
		{
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( player ) + 0x3334 ) = reinterpret_cast < std::uint32_t >( cmd );

			*prediction_random_seed = cmd ? cmd->random_seed : -1;
			*prediction_player = reinterpret_cast< int >( player );
		}

		n_interfaces::move_helper->set_host( player );

		n_interfaces::game_movement->start_track_prediction_errors( player );

		if ( cmd->weapon_select != 0 ) {
			C_BaseCombatWeapon* weapon = reinterpret_cast< C_BaseCombatWeapon* >( n_interfaces::entity_list->get_client_entity( cmd->weapon_select ) );

			if ( weapon ) {
				CCSWeaponData* weapon_data = weapon->get_weapon_data( );

				if ( weapon_data )
					player->select_item( weapon_data->weapon_name, cmd->weapon_sub_type );
			}
		}

		int v16 = *reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( player ) + 0x32F0 );
		C_BaseEntity* vehicle = nullptr;

		if ( v16 != -1 && player->get_h_vehicle( ).is_valid( ) /*&& player->get_h_vehicle( ).get( ) != nullptr*/ )
			vehicle = reinterpret_cast< C_BaseEntity* >( n_interfaces::entity_list->get_client_entity_from_handle( player->get_h_vehicle( ) ) );

		if ( cmd->impulse && ( !vehicle || player->using_standard_weapons_in_vechile( ) ) )
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( player ) + 0x31FC ) = cmd->impulse;
		
		// CBasePlayer::UpdateButtonState
		{
			int v19 = cmd->buttons;
			int *unknown = reinterpret_cast< int* >( reinterpret_cast< std::uint32_t >( player ) + 0x31F8 );
			int v20 = v19 ^ *unknown;
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( player ) + 0x31EC ) = *unknown;
			*unknown = v19;
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( player ) + 0x31F0 ) = v19 & v20;
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( player ) + 0x31F4 ) = v20 & ~v19;
		}

		player->set_local_view_angles( cmd->view_angles );

		// CPlayerMove::RunPreThink
		{
			if ( player->physics_run_think( 0 ) )
				player->pre_think( );
		}

		// CPlayerMove::RunThink
		{
			int *thinktick = reinterpret_cast< int* >( reinterpret_cast< std::uint32_t >( player ) + 0x0FC );
			if ( *thinktick != -1 && *thinktick > 0 && *thinktick <= player->get_tick_base( ) ) {
				*thinktick = -1;
				
				static auto unknown_fn = reinterpret_cast< void( __thiscall* )( int ) >( n_utilities::pattern_scan( "client_panorama.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B C6 C1 E8 16" ) );
				unknown_fn( 0 );

				player->think( );
			}
		}

		n_interfaces::prediction->setup_move( player, cmd, n_interfaces::move_helper, reinterpret_cast< CMoveData* >( move_data ) );

		if ( !vehicle )
			n_interfaces::game_movement->process_movement( player, reinterpret_cast< CMoveData* >( move_data ) );
		// else - cbf
			// ( *( void( __thiscall ** )( int, _DWORD*, void* ) )( *(_DWORD* )pVechile + 20 ) )( vehicle, player, move_data );

		n_interfaces::prediction->finish_move( player, cmd, reinterpret_cast< CMoveData* >( move_data ) );

		n_interfaces::move_helper->process_impacts( );

		// CPlayerMove::RunPostThink
		{
			// rebuilt post_think to not call ItemPostFrame to prevent double shooting - player->post_think( );
			auto post_think = [ ]( C_CSPlayer* player ) -> int {
				n_utilities::get_virtual_function< void( __thiscall* )( void* ) >( n_interfaces::model_cache, 33 )( n_interfaces::model_cache );

				if ( player->is_alive( ) /* index 155 for vfunc */ || *reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( player ) + 0x3A81 ) ) {
					n_utilities::get_virtual_function< void( __thiscall* )( void* ) >( player, 339 )( player );

					if ( player->get_flags( ) & 1 ) // FL_ONGROUND
						*reinterpret_cast< std::uintptr_t* >( std::uintptr_t( player ) + 0x3014 ) = 0;

					if ( *reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( player ) + 0x28BC ) == -1 ) // GetSequence( ) == -1
						n_utilities::get_virtual_function< void( __thiscall* )( void*, int ) >( player, 218 )( player, 0 ); // SetSequence( 0 )

					n_utilities::get_virtual_function< void( __thiscall* )( void* ) >( player, 219 )( player ); // StudioFrameAdvance( )

					static auto post_think_v_physics = reinterpret_cast< bool( __thiscall* )( C_BaseEntity* ) >( n_utilities::pattern_scan( "client_panorama.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB ? ? ? ? ? 75 50" ) );
					post_think_v_physics( player );
				}

				static auto simulate_player_simulated_entities = reinterpret_cast< bool( __thiscall* )( C_BaseEntity* ) >( n_utilities::pattern_scan( "client_panorama.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72 90 8B 86" ) );
				simulate_player_simulated_entities( player );

				return n_utilities::get_virtual_function< int( __thiscall* )( void* ) >( n_interfaces::model_cache, 34 )( n_interfaces::model_cache );
			};

			post_think( player );
		}
	}

	void unload( C_CSPlayer* player ) {
		n_interfaces::game_movement->finish_track_prediction_errors( player );

		n_interfaces::move_helper->set_host( 0 );

		n_interfaces::game_movement->reset( );

		// CPlayerMove::FinishCommand
		{
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( player ) + 0x3334 ) = 0;

			*prediction_random_seed = -1;
			*prediction_player = 0;
		}

		if ( n_interfaces::global_vars_base->frame_time > 0.f )
			player->get_tick_base( )++;

		n_interfaces::global_vars_base->current_time = old_current_time;
		n_interfaces::global_vars_base->frame_time = old_frame_time;
	}
}