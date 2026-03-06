//
#pragma once

/////////
#if(defined MINDPOWER_USE_DLL || defined USE_MINDPOWER)

#include "MindPowerAPI.h"
#define LW_FRONT_API MINDPOWER_API

#else
//////////////
#ifdef LW_FRONT_USE_DLL

#ifdef LW_FRONT_EXPORTS
	#define LW_FRONT_API __declspec(dllexport)
#else
	#define LW_FRONT_API __declspec(dllimport)
#endif

#else

#define LW_FRONT_API

#endif

#endif
