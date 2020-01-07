#pragma once

#include "utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/iclientrenderable.h

class IClientUnknown;

class IClientRenderable {
public:
	// gets at the containing class...
	virtual_fn( get_client_unknown( void ), 0, IClientUnknown* ( __thiscall* )( void* ) ); // 0
};