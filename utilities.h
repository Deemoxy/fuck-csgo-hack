#pragma once

#include "common_includes.h"

#include <cstdint>

namespace n_utilities {
	bool initialize_console( const char* window_name );

	bool unload_console( void );

	void* capture_interface( const char* module_name, const char* interface_name );

	std::uint8_t* pattern_scan( const char* module_name, const char* signature );

	template < typename T > static T get_virtual_function( void* base, const std::uint16_t index ) {
		return ( *reinterpret_cast< T** >( base ) )[ index ];
	}
}

#define virtual_fn( function_name, index, type, ... ) \
	auto function_name { \
		return n_utilities::get_virtual_function< type >( this, index )( this, __VA_ARGS__ ); \
	};