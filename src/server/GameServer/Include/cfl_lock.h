#pragma once

#ifndef _CFL_LOCK_H_
#define _CFL_LOCK_H_

#include <windows.h>

class cfl_nilock
    {
public:
    cfl_nilock() {}
    virtual ~cfl_nilock() {}

public:
    virtual void lock() {}
    virtual bool trylock() {return true;}
    virtual void unlock() {}
    };

class cfl_spinlock : public cfl_nilock
    {
public:
    cfl_spinlock();
    ~cfl_spinlock();

public:
    void lock();
    bool trylock();
    void unlock();

private:
    CRITICAL_SECTION _cs;};

#endif
