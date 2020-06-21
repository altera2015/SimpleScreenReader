#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <Windows.h>
#include <string>

class Mutex
{
    HANDLE m_Mutex;
public:
    Mutex( const std::wstring & name );
    ~Mutex();
    bool valid();
    bool lock(DWORD ms = INFINITE);
    void unlock();
};

class MutexScope 
{
    Mutex & m_Mutex;
public:
    MutexScope(Mutex & mutex, bool alreadyLocked = false);
    ~MutexScope();
};

#endif
