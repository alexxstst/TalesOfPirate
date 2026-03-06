#pragma once

/**
 *  @file matrix_stuff.h
 *
 *  matrix_stuff.h, v 1.0 2004/9/22 13:14:05 shanghai China
 *
 *  @author claude Fan <fanyf2002@hotmail.com>
 *
 */

#ifndef _MATRIX_STUFF_H_
#define _MATRIX_STUFF_H_

#include <stdio.h>

#if defined(_MSC_VER)
    #if _MSC_VER >= 1020
        #include <stdexcept>
    #else
        #include <stdexcpt.h>
    #endif    
#elif (__GNUC__ >= 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 8))
    #include <stdexcept>
#else
    #include <stdexcep>
#endif

template <class T>
class matrix_stuff
    {
public:
    // constructors, destructor, assignment operators
    matrix_stuff(size_t row, size_t col);
    matrix_stuff(size_t circle);
    matrix_stuff(matrix_stuff<T> const& m);
    matrix_stuff<T>& operator =(matrix_stuff<T> const& m);
    ~matrix_stuff();

    // value extraction method
    size_t ROW() const {return _m->_row;}
    size_t COL() const {return _m->_col;}
    size_t ESZ() const {return sizeof(T);}

    // subscript operator
    T* operator ()(size_t row);
    T& operator ()(size_t row, size_t col);
    T  operator ()(size_t row, size_t col) const;

    // resize matrix
    void resize(size_t row, size_t col);
    void resize(size_t circle);

    // set null to all elements
    void null();

protected:
    struct core_matrix
        {
        T** _vp;
        size_t _row;
        size_t _col;
        size_t _row_size;
        size_t _col_size;
        int _ref_cnt;

        core_matrix(size_t row, size_t col, T** vp)
            {
            _row = _row_size = row;
            _col = _col_size = col;
            _ref_cnt = 1;

            _vp = new T* [row];
            size_t row_len = col * sizeof(T);

            for (size_t i = 0; i < row; ++ i)
                {
                _vp[i] = new T [col];
                if (vp) memcpy(_vp[i], vp[i], row_len);
                }}
        ~core_matrix()
            {
            for (size_t i = 0; i < _row_size; ++ i)
                delete [] _vp[i];
            delete [] _vp;}
        } *_m;
    void _clone();
    void _realloc(size_t row, size_t col);
    };

// constructors, destructor, assignment operators
template <class T>
inline matrix_stuff<T>::matrix_stuff(size_t row, size_t col)
    {_m = new core_matrix(row, col, NULL);}

template <class T>
inline matrix_stuff<T>::matrix_stuff(size_t circle)
    {size_t row_col = 1 + 2 * circle;
    _m = new core_matrix(row_col, row_col, NULL);}

template <class T>
inline matrix_stuff<T>::matrix_stuff(matrix_stuff<T> const& m)
    {_m = m._m; ++ _m->_ref_cnt;}

template <class T>
inline matrix_stuff<T>& matrix_stuff<T>::operator =(matrix_stuff<T> const& m)
    {++ m._m->_ref_cnt;
    if (-- _m->_ref_cnt == 0) delete _m;
    _m = m._m; return *this;}

template <class T>
inline matrix_stuff<T>::~matrix_stuff()
    {if (-- _m->_ref_cnt == 0) delete _m;}

// internal copy constructor
template <class T>
inline void matrix_stuff<T>::_clone()
    {-- _m->_ref_cnt;
    _m = new core_matrix(_m->_row, _m->_col, _m->_vp);}

// reallocation method
template <class T>
inline void matrix_stuff<T>::_realloc(size_t row, size_t col)
    {
    if (row == _m->_row_size && col == _m->_col_size)
        {
        _m->_row = _m->_row_size;
        _m->_col = _m->_col_size; return;
        }

    core_matrix* m1 = new core_matrix(row, col, NULL);
    size_t min_col = min(_m->_col, col) * sizeof(T);
    size_t min_row = min(_m->_row, row);

    for (size_t i = 0; i < min_row; ++ i)
        memcpy(m1->_vp[i], _m->_vp[i], min_col);

    if (-- _m->_ref_cnt == 0)
        delete _m;

    _m = m1;}

// subscript operator
template <class T>
inline T* matrix_stuff<T>::operator ()(size_t row)
    {
    if (row >= _m->_row)
        throw std::logic_error("matrix_stuff<T>::operator(): index out of range!");
    if (_m->_ref_cnt > 1) _clone(); // copy on possiblely write
    return _m->_vp[row];}

template <class T>
inline T& matrix_stuff<T>::operator ()(size_t row, size_t col)
    {
    if (row >= _m->_row || col >= _m->_col)
        throw std::logic_error("matrix_stuff<T>::operator(): index out of range!");
    if (_m->_ref_cnt > 1) _clone(); // copy on possiblely write
    return _m->_vp[row][col];}

template <class T>
inline T matrix_stuff<T>::operator ()(size_t row, size_t col) const
    {
    if (row >= _m->_row || col >= _m->_col)
        throw std::logic_error("matrix_stuff<T>::operator(): index out of range!");
    return _m->_vp[row][col];}

// resize matrix
template <class T>
inline void matrix_stuff<T>::resize(size_t row, size_t col)
    {
    size_t i, j;
    size_t old_row = _m->_row;
    size_t old_col = _m->_col;

    if (row != _m->_row_size || col != _m->_col_size)
        _realloc((void*)row, col);

    for (i = old_row; i < row; ++ i)
        for (j = 0; j < col; ++ j)
            _m->_vp[i][j] = T(0);

    for (i = 0; i < row; ++ i)
        for (j = old_col; j < col; ++ j)
            _m->_vp[i][j] = T(0);
    }

template <class T>
inline void matrix_stuff<T>::resize(size_t circle)
    {
    size_t row_col = 1 + 2 * circle;
    resize(row_col, row_col);}

// set null to all elements
template <class T>
inline void matrix_stuff<T>::null()
    {
    if (_m->_ref_cnt > 1) _clone();
    for (size_t i = 0; i < _m->_row; ++ i)
        for (size_t j = 0; j < _m->_col; ++ j)
            _m->_vp[i][j] = T(0);
    }

#endif // _MATRIX_STUFF_H_
