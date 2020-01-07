#include "hooks.h"

namespace n_hooks {
	void __fastcall n_functions::DrawModelExecute( std::uintptr_t ecx, std::uintptr_t edx, void* context, void* state, ModelRenderInfo_t& info, void* custom_bone_to_world ) {
		static auto original_fn = n_hooks::model_render->get_original_function< decltype( &DrawModelExecute ) >( 21 );

		C_CSPlayer* model_entity = reinterpret_cast< C_CSPlayer* >( n_interfaces::entity_list->get_client_entity( info.entity_index ) );

		const char* const model_name = n_interfaces::model_info->get_model_name( info.model );

		// this will be moved to its own file if/when this cpp file becomes more clustered
		auto player_chams = [ ]( C_CSPlayer* model_entity, ModelRenderInfo_t& info, const char* const model_name ) {
			if ( !std::strstr( model_name, "models/player" ) )
				return;

			if ( !model_entity || !model_entity->is_alive( ) || !info.model )
				return;

			static IMaterial* material = n_interfaces::material_system->get_material( "debug/debugdrawflat", "Model textures" );

			if ( !material )
				return;

			const int health = model_entity->get_health( );
			const float r = ( 255.f - health * 2.55f ) / 255.f ;
			const float g = health * 2.55f / 255.f;
			const float b = 0.f;

			material->color_modulate( r, g, b );
			material->alpha_modulate( 1.f );
			material->set_material_var_flag( MATERIAL_VAR_IGNOREZ, true );

			n_interfaces::model_render->forced_material_override( material );
		};

		player_chams( model_entity, info, model_name );

		original_fn( ecx, edx, context, state, info, custom_bone_to_world );
		n_interfaces::model_render->forced_material_override( nullptr );
	}
}