#pragma once

#include <Windows.h>

class CCriticalSectionWrapper
{
public:
    CCriticalSectionWrapper()  { InitializeCriticalSection(&m_csMutex); }
    ~CCriticalSectionWrapper() { DeleteCriticalSection(&m_csMutex); }
    void Acquire() { EnterCriticalSection(&m_csMutex); }
    void Release() { LeaveCriticalSection(&m_csMutex); }
private:
    CRITICAL_SECTION m_csMutex;
};

class CCriticalSectionGuard
{
public:
    CCriticalSectionGuard(CCriticalSectionWrapper& csMutex) : m_csMutex(csMutex) { m_csMutex.Acquire(); }
    ~CCriticalSectionGuard() { m_csMutex.Release(); }
private:
    CCriticalSectionWrapper& m_csMutex;
};

template<typename TYPE, typename LOCK>
class CSynchSingleton
{
public:
    // Global access point to the Singleton.
    static TYPE* instance(void)
    {
        if(NULL == CSynchSingleton<TYPE, LOCK>::m_pSingleton)
        {
            static LOCK InstanceMutex;
            CCriticalSectionGuard InstanceGuard(InstanceMutex);
            if(NULL == CSynchSingleton<TYPE, LOCK>::m_pSingleton)
                CSynchSingleton<TYPE, LOCK>::m_pSingleton = new CSynchSingleton<TYPE, LOCK>();
        }
        InterlockedIncrement(&CSynchSingleton<TYPE, LOCK>::m_pSingleton->m_lInstanceRefCnt);
        return &CSynchSingleton<TYPE, LOCK>::m_pSingleton->m_Instance;
    }

    // Cleanup method, used to destroy the instance.
    static void cleanup()
    {
        InterlockedDecrement(&CSynchSingleton<TYPE, LOCK>::m_pSingleton->m_lInstanceRefCnt);
        if(0 == CSynchSingleton<TYPE, LOCK>::m_pSingleton->m_lInstanceRefCnt)
        {
            if(CSynchSingleton<TYPE, LOCK>::m_pSingleton)
            {
                delete CSynchSingleton<TYPE, LOCK>::m_pSingleton;
                CSynchSingleton<TYPE, LOCK>::m_pSingleton = NULL;
            }
        }
    }

protected:
    // Contained instance.
    TYPE m_Instance;
    long m_lInstanceRefCnt;
    static CSynchSingleton<TYPE, LOCK>* m_pSingleton;
};

template<typename TYPE, typename LOCK> CSynchSingleton<TYPE, LOCK>* CSynchSingleton<TYPE, LOCK>::m_pSingleton = NULL;