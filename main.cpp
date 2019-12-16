#include "common_includes.h"
#include "utilities.h"
#include "interfaces.h"
#include "netvar_manager.h"
#include "render.h"
#include "animations.h"
#include "hooks.h"

#include <thread>
#include <chrono>
#include <stdexcept>

bool unload( void ) {
#ifdef _DEBUG
	n_utilities::unload_console( );
#endif

	g_animations.unload( );

	if ( !n_hooks::unload( ) )
		return false;

	return true;
}

unsigned long __stdcall initialize_cheat( void* argument ) {
	try {
#ifdef _DEBUG
		if ( !n_utilities::initialize_console( "debug" ) )
			std::abort( );

		printf( "initializing cheat... \n" );

		if ( !n_interfaces::initialize( ) )
			throw std::runtime_error( "failed to initialize interfaces" );

		if ( !n_netvar_tree::initialize( ) )
			throw std::runtime_error( "failed to populate netvar tree" );

		if ( !n_render::initialize( ) )
			throw std::runtime_error( "failed to initialize renderer" );

		g_animations.initialize( );

		if ( !n_hooks::initialize( ) )
			throw std::runtime_error( "failed to initialize hooks" );
#endif
	}
	catch ( const std::runtime_error& exception ) {
#ifdef _DEBUG
		printf( exception.what( ) );

		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

		if ( n_interfaces::initialization_success && n_hooks::initialization_success ) {
			if ( !unload( ) )
				std::abort( );

			FreeLibraryAndExitThread( reinterpret_cast< HMODULE >( argument ), 0 );
		}
		else {
			std::abort( );
		}
#endif
	}

	return 1;
}

unsigned long __stdcall unload_cheat( void* argument ) {
	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 25 ) );

	if ( !unload( ) )
		std::abort( );

	Beep( 500, 350 );

	FreeLibraryAndExitThread( static_cast< HMODULE >( argument ), 0 );
}

int __stdcall DllMain( const HMODULE hinst_dll, const unsigned long reason_for_call, const void* reserved ) {
	if ( hinst_dll )
		DisableThreadLibraryCalls( hinst_dll );

	if ( reason_for_call == 1 ) {
		auto cheat_thread = CreateThread( 0, 0, initialize_cheat, 0, 0, 0 );

		if ( !cheat_thread )
			return 0;

		auto free_thread = CreateThread( 0, 0, unload_cheat, hinst_dll, 0, 0 );

		if ( !free_thread )
			return 0;

		CloseHandle( cheat_thread );
		CloseHandle( free_thread );

		return 1;
	}

	return 0;
}