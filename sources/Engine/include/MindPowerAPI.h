//
#pragma once

/////////

////////////
#ifdef MINDPOWER_USE_DLL

#ifdef MINDPOWER_EXPORTS
	#define MINDPOWER_API __declspec(dllexport)
#else
	#define MINDPOWER_API __declspec(dllimport)
#endif

#else

#define MINDPOWER_API

#endif

