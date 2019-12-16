#pragma once

#include "interfaces.h"

#include <string>

namespace n_render {
	namespace n_font {
		extern HFont main;

		bool initialize( void );
	}

	extern int screen_w, screen_h;

	void filled_rect( int x, int y, int w, int h, int r, int g, int b, int a );
	void outlined_rect( int x, int y, int w, int h, int r, int g, int b, int a );
	void line( int x, int y, int w, int h, int r, int g, int b, int a );
	void text( HFont font, std::string text, int x, int y, bool centered, int r, int g, int b, int a );

	bool initialize( void );
}