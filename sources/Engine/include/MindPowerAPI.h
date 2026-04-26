//
#pragma once


#ifdef MINDPOWER_USE_DLL

#ifdef MINDPOWER_EXPORTS
#define __declspec(dllexport)
#else
#define __declspec(dllimport)
#endif

#else

#define MINDPOWER_API

#endif
