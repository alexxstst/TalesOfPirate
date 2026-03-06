//
#pragma once

#include "lwHeader.h"
#include "lwInterfaceExt.h"
#include "lwStdInc.h"
#include "lwDirectX.h"
#include "lwClassDecl.h"

LW_BEGIN

// this is a min-heap class
// if you want to get max-heap, pls wrap operator<() of T

template<typename T>
class lwHeapT
{
public:
    typedef T value_type;
    typedef value_type* value_type_ptr;
    typedef value_type& value_type_ref;
    typedef BOOL (*lwHeapTCompareProc)(const T& i, const T& j);
    typedef void (*lwHeapTFilterProc)(const T& t, DWORD i);

    enum { DEFAULT_CHUNK_SIZE = 32 };

private:
    T* _buf;
    DWORD _buf_size;
    DWORD _num;

    lwHeapTCompareProc _comp_proc;
    lwHeapTFilterProc _filter_proc;

private:
    DWORD _Parent(DWORD i) { return (i - 1) / 2; }
    DWORD _Left(DWORD i) { return (2 * i + 1); }
    DWORD _Right(DWORD i) { return (2 * i + 2); }
    void _Place(DWORD i, const T& t) 
    { 
        _buf[i] = t; 
        if(_filter_proc) { (*_filter_proc)(t, i); }
    }
    void _Swap(DWORD i, DWORD j) { T t = _buf[i]; _buf[i] = _buf[j]; _buf[j] = t; }
    T _Value(DWORD i) { return _buf[i]; }
    T* _Ptr(DWORD i) { return &_buf[i]; }
    T& _Ref(DWORD i) { return _buf[i]; }
    BOOL _Comp(const T& i, const T& j)
    {
        return _comp_proc ? (*_comp_proc)(i, j) : (i) <= (j);
    }


    BOOL _CheckBufferState() { return (_num < _buf_size); }
    LW_RESULT _Allocate(DWORD buf_size);
    LW_RESULT _Free();
    DWORD _FilterDown(DWORD i);
    DWORD _FilterUp(DWORD i);
public:
    lwHeapT()
        : _buf(0), _buf_size(0), _num(0), _comp_proc(0), _filter_proc(0)
    {}
    ~lwHeapT()
    {
        Clear();
    }

    LW_RESULT Reserve(DWORD size);
    LW_RESULT Clear();
    LW_RESULT Push(const T& t);
    LW_RESULT Push(const T* buf, DWORD size);
    LW_RESULT Pop();
    LW_RESULT Top(T* t);
    LW_RESULT Find(DWORD* id, const T& t);
    LW_RESULT Update(DWORD id);
    LW_RESULT Remove(DWORD id);
    LW_RESULT Copy(lwHeapT* src);

    void SetCompareProc(lwHeapTCompareProc proc) { _comp_proc = proc; }
    void SetFilterProc(lwHeapTFilterProc proc) { _filter_proc = proc; }
    lwHeapTCompareProc GetCompareProc() { return _comp_proc; }
    lwHeapTFilterProc GetFilterProc() { return _filter_proc; }
    DWORD GetHeapNum() { return _num; }
    DWORD GetHeapCapacity() { return _buf_size; }
    T* GetBuffer() { return _buf; }


};


template<typename T>
LW_RESULT lwHeapT<T>::_Allocate(DWORD buf_size)
{
    if(buf_size < _buf_size)
        return LW_RET_FAILED;

    int size = (buf_size == 0xffffffff) ? DEFAULT_CHUNK_SIZE : (int)buf_size;

#if(defined USE_NEW_PLACEMENT)
    int new_size;
    // i cannot understand the error by ::new(&_buf[_buf_size+i]) T; ???
    if(_buf_size == 0)
    {
        _buf = (T*)LW_MALLOC(sizeof(T) * size);
    }
    else
    {
        while(size <= (int)_buf_size)
        {
            size *= 2;
        }
        _buf = (T*)LW_REALLOC(_buf, sizeof(T) * size);
    }

    new_size = size - _buf_size;

    for(int i = 0; i < new_size; i++)
    {
        ::new(&_buf[_buf_size+i]) T;
    }

    _buf_size = size;
#else
    if(_buf_size == 0)
    {
        _buf = LW_NEW(T[size]);
    }
    else
    {
        T* new_buf;

        while(size <= (int)_buf_size)
        {
            size *= 2;
        }
        new_buf = LW_NEW(T[size]);

        for(DWORD i = 0; i < _buf_size; i++)
        {
            new_buf[i] = _buf[i];
        }

        LW_DELETE_A(_buf);
        _buf = new_buf;
    }

    _buf_size = size;
#endif

    return LW_RET_OK;
}
template<typename T>
LW_RESULT lwHeapT<T>::_Free()
{

    if(_buf_size > 0)
    {
#if(defined USE_NEW_PLACEMENT)
        for(DWORD i = 0; i < _buf_size; i++)
        {
            _buf[i].~T();
        }
        LW_FREE(_buf);
#else
        LW_DELETE_A(_buf);
#endif
        _buf = 0;
        _buf_size = 0;
        _num = 0;
    }

    return LW_RET_OK;
}
template<typename T>
DWORD lwHeapT<T>::_FilterDown(DWORD i)
{
    T t = _Value(i);
    DWORD index = i;
    DWORD l = _Left(i);
    DWORD r = _Right(i);
    DWORD c;

    while(l < GetHeapNum())
    {
        if(r < GetHeapNum() && _Comp(_Ref(r), _Ref(l)))
            c = r;
        else
            c = l;

        if(_Comp(t, _Ref(c)))
            break;

        _Place(index, _buf[c]);
        //_buf[index] = _buf[c];

        index = c;
        l = _Left(index);
        r = _Right(index);
    }

    if(index != i)
    {
        _Place(index, t);
        //_buf[index] = t;
    }

    return index;
}

template<typename T>
DWORD lwHeapT<T>::_FilterUp(DWORD i)
{
    T t = _Value(i);
    DWORD index = i;
    DWORD p = _Parent(i);

    while((index > 0) && _Comp(t, _Ref(p)))
    {
        _Place(index, _buf[p]);
        //_buf[index] = _buf[p];

        index = p;
        p = _Parent(p);
    }

    if(index != i)
    {
        _Place(index, t);
        //_buf[index] = t;
    }

    return index;
}


template<typename T>
LW_RESULT lwHeapT<T>::Reserve(DWORD size)
{
    return _Allocate(size);
}

template<typename T>
LW_RESULT lwHeapT<T>::Clear()
{
    return _Free();
}

template<typename T>
LW_RESULT lwHeapT<T>::Push(const T& t)
{
    if(_num >= _buf_size)
    {
        if(LW_FAILED(_Allocate(0xffffffff)))
            return LW_RET_FAILED;
    }

    _Place(_num, t);
    _FilterUp(_num);
    _num += 1;
    
    return LW_RET_OK;
}
template<typename T>
LW_RESULT lwHeapT<T>::Push(const T* buf, DWORD size)
{
    for(DWORD i = 0; i < size; i++)
    {
        Push(buf[i]);
    }

    return LW_RET_OK;
}

template<typename T>
LW_RESULT lwHeapT<T>::Pop()
{
    if(_num == 0)
        return LW_RET_FAILED;

    _num -= 1;

    if(_num > 0)
    {
        _Place(0, _Value(_num));
        _FilterDown(0);
    }

    return LW_RET_OK;
}
template<typename T>
LW_RESULT lwHeapT<T>::Top(T* t)
{
    if(_num == 0)
        return LW_RET_FAILED;

    *t = _Value(0);

    return LW_RET_OK;
}
template<typename T>
LW_RESULT lwHeapT<T>::Find(DWORD* id, const T& t)
{
    if(_num == 0)
        return LW_RET_FAILED;

    for(DWORD i = 0; i < GetHeapNum(); i++)
    {
        if(_buf[i] == t)
        {
            *id = i;
            return LW_RET_OK;
        }
    }

    return LW_RET_FAILED;
}
template<typename T>
LW_RESULT lwHeapT<T>::Update(DWORD id)
{
    if(id >= _num)
        return LW_RET_FAILED;
  
    if((id > 0) && _Comp(_Ref(id), _Ref(_Parent(id))))
    {
        _FilterUp(id);
    }
    else
    {
        _FilterDown(id);
    }

    return LW_RET_OK;
}
template<typename T>
LW_RESULT lwHeapT<T>::Remove(DWORD id)
{
    if(id >= _num)
        return LW_RET_FAILED;

    _num -= 1;

    if(id < _num)
    {
        _Place(id, _buf[_num]);

        return Update(id);
    }
    
    return LW_RET_OK;
}
template<typename T>
LW_RESULT lwHeapT<T>::Copy(lwHeapT* src)
{
    Clear();

    _num = src->_num;
    _buf_size = src->_buf_size;
    _comp_proc = src->_comp_proc;
    _filter_proc = src->_filter_proc;
    _buf = LW_NEW(T[_buf_size]);
    memcpy(_buf, src->_buf, sizeof(T) * _num);

    return LW_RET_OK;
}

//
class lwHeap : public lwIHeap
{
    LW_STD_DECLARATION();

    typedef lwHeapT<void*> T;

private:
    T _heap;

public:
    LW_RESULT Reserve(DWORD size) 
    { return _heap.Reserve(size); }
    LW_RESULT Clear() 
    { return _heap.Clear(); }
    LW_RESULT Push(const void* t) 
    { return _heap.Push((T::value_type_ref)t); }
    LW_RESULT Push(const void* buf, DWORD size) 
    { return _heap.Push((T::value_type_ptr)buf, size); }
    LW_RESULT Pop() 
    { return _heap.Pop(); }
    LW_RESULT Top(void** t) 
    { return _heap.Top(t); }
    LW_RESULT Find(DWORD* id, const void* t) 
    { return _heap.Find(id, (T::value_type_ref)t); }
    LW_RESULT Update(DWORD id) 
    { return _heap.Update(id); }
    LW_RESULT Remove(DWORD id)
    { return _heap.Remove(id); }

    void SetCompareProc(lwHeapCompProc proc) 
    { _heap.SetCompareProc((T::lwHeapTCompareProc)proc); }
    void SetFilterProc(lwHeapFilterProc proc) 
    { _heap.SetFilterProc((T::lwHeapTFilterProc)proc); }
    lwHeapCompProc GetCompareProc() 
    { return (lwHeapCompProc)_heap.GetCompareProc(); }
    lwHeapFilterProc GetFilterProc() 
    { return (lwHeapFilterProc)_heap.GetFilterProc(); }

    DWORD GetHeapNum() 
    { return _heap.GetHeapNum(); }
    DWORD GetHeapCapacity() 
    { return _heap.GetHeapCapacity(); }
    void* GetBuffer() 
    { return _heap.GetBuffer(); }

    LW_RESULT Clone(lwIHeap** out_heap);
    
};

LW_END
