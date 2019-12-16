#include "hooks.h"

#include "interfaces.h"

namespace n_hooks {
	bool initialization_success = false;

	std::unique_ptr< c_vmt > client_mode;
	std::unique_ptr< c_vmt > engine_vgui;

	bool initialize( void ) {
		client_mode = std::make_unique< c_vmt >( );
		engine_vgui = std::make_unique< c_vmt >( );

		/* IClientMode */
		if ( !client_mode->initialize( n_interfaces::client_mode ) )
			return false;

		if ( !client_mode->hook_function( 24, n_functions::CreateMove ) )
			return false;

		if ( !client_mode->hook_function( 35, n_functions::GetViewModelFOV ) )
			return false;

		/* IVEngineVGui */
		if ( !engine_vgui->initialize( n_interfaces::engine_vgui ) )
			return false;

		if ( !engine_vgui->hook_function( 14, n_functions::Paint ) )
			return false;

		initialization_success = true;

		return true;
	}

	bool unload( void ) {
		/* IClientMode */
		if ( !client_mode->unhook( ) )
			return false;

		/* IVEngineVGui */
		if ( !engine_vgui->unhook( ) )
			return false;

		return true;
	}
}