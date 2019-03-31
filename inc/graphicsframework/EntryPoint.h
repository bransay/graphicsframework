#ifndef GF_ENTRYPOINTS_H_
#define GF_ENTRYPOINTS_H_

#include "graphicsframework/Config.h"

// includes the correct entry point based on the platform
#ifdef WIN32
	#include "main_win32.h"
#endif

#endif
