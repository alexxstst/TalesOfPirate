//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"

LW_BEGIN

enum
{
    ERR_LWIOBJECTPOOL_POOL_FULL = -2,
    ERR_LWIOBJECTPOOL_INVALID_ID = -3,
    ERR_LWIOBJECTPOOL_EXISTED_ID = -4,
};

template< typename Tp, DWORD size >
class lwObjectPool
{
    struct ObjRef 
    {
        Tp obj;
        DWORD ref;
    };

public:
    enum {
        POOL_SIZE = size
    };

protected:
	ObjRef _pool_seq[ POOL_SIZE ];
	DWORD _obj_num;
    DWORD _next_id;

public:
    inline lwObjectPool()
        : _obj_num( 0 ), _next_id( 0 )
    {
        memset( _pool_seq, 0, sizeof(ObjRef) * POOL_SIZE );
    }
    inline ~lwObjectPool() 
    {}

    LW_RESULT Register( DWORD* ret_id, Tp obj );
	LW_RESULT Register( Tp obj, DWORD id );
	LW_RESULT Unregister( Tp* ret_obj, DWORD id );
    LW_RESULT AddRef( DWORD id, DWORD ref_cnt );
    LW_RESULT GetObj( Tp* ret_obj, DWORD id );
    Tp QueryObj( DWORD id );
    DWORD GetRef( DWORD id );

    DWORD FindObject( Tp obj );
    inline void Clear() 
    { 
        memset(_pool_seq, 0, sizeof(ObjRef) * POOL_SIZE);
        _obj_num = 0;
        _next_id = 0;
    }


    inline int GetObjNum() { return _obj_num; }

};

template< typename Tp, DWORD size >
LW_RESULT lwObjectPool< Tp, size>::Register( DWORD* ret_id, Tp obj )
{
    if( _obj_num == POOL_SIZE )
        return ERR_LWIOBJECTPOOL_POOL_FULL;

	for( ; _pool_seq[ _next_id ].ref != 0; _next_id++ ) 
    {
		if( _next_id >= POOL_SIZE )
        {
			_next_id = _next_id % POOL_SIZE;
        }
	}

	DWORD id = _next_id;

	if( ++_next_id == POOL_SIZE )
		_next_id = 0;

	_pool_seq[ id ].obj = obj;
    _pool_seq[ id ].ref = 1;

	_obj_num += 1;

    *ret_id = id;

    return LW_RET_OK;
}

template< typename Tp, DWORD size >
LW_RESULT lwObjectPool< Tp, size >::Register( Tp obj, DWORD id )
{
	if( id < 0 || id >= POOL_SIZE )
		return ERR_LWIOBJECTPOOL_INVALID_ID;

	if( _pool_seq[ id ].ref > 0 )
		return ERR_LWIOBJECTPOOL_EXISTED_ID;

	_pool_seq[ id ].obj = obj;
    _pool_seq[ id ].ref = 1;

	_obj_num += 1;

	return 1;
}

template< typename Tp, DWORD size >
LW_RESULT lwObjectPool< Tp, size >::Unregister( Tp* ret_obj, DWORD id )
{
    LW_RESULT ret = LW_RET_OK;

	if( id < 0 || id >= POOL_SIZE )
    {
		ret = ERR_LWIOBJECTPOOL_INVALID_ID;
        goto __ret;
    }
    {

        ObjRef* o = &_pool_seq[id];

        if (o->ref == 0)
        {
            ret = ERR_LWIOBJECTPOOL_INVALID_ID;
            goto __ret;
        }

        o->ref -= 1;

        if (o->ref == 0)
        {
            if (ret_obj)
            {
                *ret_obj = o->obj;
            }
            //o->obj = 0;
            _obj_num -= 1;
            _next_id = id;

            ret = LW_RET_OK_1;
        }
    }
__ret:
	return ret;
}

template< typename Tp, DWORD size >
LW_RESULT lwObjectPool< Tp, size >::AddRef( DWORD id, DWORD ref_cnt )
{
	if( id < 0 || id >= POOL_SIZE )
		return ERR_LWIOBJECTPOOL_INVALID_ID;

    ObjRef* o = &_pool_seq[ id ];
    if( o->ref == 0 )
        return ERR_LWIOBJECTPOOL_INVALID_ID;

    o->ref += 1;

    return LW_RET_OK;

}

template< typename Tp, DWORD size >
LW_RESULT lwObjectPool< Tp, size >::GetObj( Tp* ret_obj, DWORD id )
{
    if( id < 0 || id >= POOL_SIZE )
        return ERR_LWIOBJECTPOOL_INVALID_ID;

    ObjRef* o = &_pool_seq[ id ];

    if( o->ref == 0 )
        return ERR_LWIOBJECTPOOL_INVALID_ID;

    *ret_obj = o->obj;

    return LW_RET_OK;
}

template< typename Tp, DWORD size >
Tp lwObjectPool< Tp, size >::QueryObj( DWORD id )
{
    if( id < 0 || id >= POOL_SIZE )
        return ERR_LWIOBJECTPOOL_INVALID_ID;

    ObjRef* o = &_pool_seq[ id ];

    return o->obj;
}


template< typename Tp, DWORD size >
DWORD lwObjectPool< Tp, size >::GetRef( DWORD id )
{
	//if( id < 0 || id >= POOL_SIZE )
	//	return ERR_LWIOBJECTPOOL_INVALID_ID;

    return _pool_seq[ id ].ref;

}

template< typename Tp, DWORD size >
DWORD lwObjectPool< Tp, size >::FindObject( Tp obj )
{
    for( DWORD i = 0; i < POOL_SIZE; i++ ) 
    {
        if( _pool_seq[ i ].obj == obj ) 
        {
            return i;
        }
    }

    return LW_INVALID_INDEX;
}

typedef lwObjectPool< void*, 1024 > lwObjectPoolVoidPtr1024;
typedef lwObjectPool< void*, 2048 > lwObjectPoolVoidPtr2048;
typedef lwObjectPool< void*, 4096 > lwObjectPoolVoidPtr4096;
typedef lwObjectPool< void*, 10240 > lwObjectPoolVoidPtr10240;
typedef lwObjectPool< void*, 40960 > lwObjectPoolVoidPtr40960;
typedef lwObjectPool< void*, 102400 > lwObjectPoolVoidPtr102400;
typedef lwObjectPool< DWORD, 10240 > lwObjectPoolDword10240;
typedef lwObjectPool< DWORD, 40960 > lwObjectPoolDword40960;

LW_END
