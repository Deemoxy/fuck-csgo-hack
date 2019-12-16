#include "hooks.h"

#include "render.h"

namespace n_hooks {
	void __fastcall n_functions::Paint( std::uintptr_t ecx, std::uintptr_t edx, int mode ) {
		static auto original_fn = n_hooks::engine_vgui->get_original_function< decltype( &Paint ) >( 14 );

		original_fn( ecx, edx, mode );

		if ( mode & PAINT_UIPANELS ) {
			// xref: "-pixel_offset_x" => sig the sub
			static auto start_drawing_fn = reinterpret_cast< void( __thiscall* )( void* ) >( n_utilities::pattern_scan( "vguimatsurface.dll", "55 8B EC 83 E4 C0 83 EC 38" ) );

			// xref: "-pixel_offset_y" => sig the sub directly below
			static auto finish_drawing_fn = reinterpret_cast< void( __thiscall* )( void* ) >( n_utilities::pattern_scan( "vguimatsurface.dll", "8B 0D ? ? ? ? 56 C6 05 ? ? ? ? ?" ) );

			start_drawing_fn( n_interfaces::surface ); {
				//n_render::rectangle( 5, 5, 100, 100, 255, 255, 255, 255 );
				n_render::text( n_render::n_font::main, "fuck", 5, 5, false, 255, 255, 255, 255 );

				n_interfaces::surface->draw_filled_rect_fade( 5, 20, 115, 40, 255, 122, true );
			}
			finish_drawing_fn( n_interfaces::surface );
		}
	}
}