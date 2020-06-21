#include "Mutex.h"


Mutex::Mutex(const std::wstring & name)
{
    if (name.length() > 0)
    {
        m_Mutex = CreateMutex(0, false, name.c_str());
    }
    else
    {
        m_Mutex = CreateMutex(0, false, 0);
    }
}


Mutex::~Mutex()
{
    CloseHandle(m_Mutex);
}

bool Mutex::valid()
{
    return m_Mutex != NULL;
}

bool Mutex::lock(DWORD ms)
{
    if (!valid())
    {
        return false;
    }

    return (WaitForSingleObject(m_Mutex, ms) == WAIT_OBJECT_0);    
}

void Mutex::unlock()
{
    if (!valid())
    {
        return;
    }
    ReleaseMutex(m_Mutex);
}


MutexScope::MutexScope(Mutex & mutex, bool alreadyLocked) : m_Mutex(mutex)
{
    if (!alreadyLocked)
    {
        m_Mutex.lock();
    }
}

MutexScope::~MutexScope()
{
    m_Mutex.unlock();
}
