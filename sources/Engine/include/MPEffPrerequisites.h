#ifndef MPEffPrerequisites_H
#define MPEffPrerequisites_H

//=============================================================================
//=============================================================================
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

// STL 
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "MindPowerAPI.h"

//=============================================================================
//=============================================================================
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef std::string String;


//=============================================================================
//=============================================================================
#define MP_AUTO_MUTEX_NAME mpMutex
#if MP_THREAD_SUPPORT
#define MP_AUTO_MUTEX mutable boost::recursive_mutex MP_AUTO_MUTEX_NAME;
#define MP_LOCK_AUTO_MUTEX boost::recursive_mutex::scoped_lock mpAutoMutexLock(MP_AUTO_MUTEX_NAME);
#define MP_MUTEX(name) mutable boost::recursive_mutex name;
#define MP_LOCK_MUTEX(name) boost::recursive_mutex::scoped_lock mpNameLock(name);
// MP_AUTO_MUTEXmpMutex
#define MP_AUTO_SHARED_MUTEX mutable boost::recursive_mutex *MP_AUTO_MUTEX_NAME;
#define MP_LOCK_AUTO_SHARED_MUTEX boost::recursive_mutex::scoped_lock mpAutoMutexLock(*OGRE_AUTO_MUTEX_NAME);
#define MP_NEW_AUTO_SHARED_MUTEX MP_AUTO_MUTEX_NAME = new boost::recursive_mutex();
#define MP_DELETE_AUTO_SHARED_MUTEX delete MP_AUTO_MUTEX_NAME;
#define MP_COPY_AUTO_SHARED_MUTEX(from) MP_AUTO_MUTEX_NAME = from;
#else
#define MP_AUTO_MUTEX 
#define MP_LOCK_AUTO_MUTEX 
#define MP_MUTEX(name)
#define MP_LOCK_MUTEX(name)
#define MP_AUTO_SHARED_MUTEX 
#define MP_LOCK_AUTO_SHARED_MUTEX
#define MP_NEW_AUTO_SHARED_MUTEX 
#define MP_DELETE_AUTO_SHARED_MUTEX 
#define MP_COPY_AUTO_SHARED_MUTEX(from) 
#endif

//=============================================================================
//=============================================================================
class MPColorValue;
class MPEffectMath;
class MPEffQuaternion;
class MPEffVector3;
class MPParticle;
class MPParticleVisualData;
class MPParticleAffector;
class MPParticleAffectorFactory;
class MPParticleEmitter;
class MPParticleEmitterFactory;
class MPParticleSystem;
class MPParticleSystemManager;
class MPParticleSystemRenderer;
class MPParticleSystemRendererFactory;
class MPRenderQueue;
class MPMaterialPtr;
class MPCameraNOLEECH;
class MPNode;
class MPStringUtil;
template <class T>
class MPMapIterator;
class MPScriptLoader;
class MPParameterInterface;

#endif
