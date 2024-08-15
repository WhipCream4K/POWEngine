#pragma once

namespace powe
{
    template<typename T>
    class ThreadSafeSingleton
    {
        // Implements DCLP (Double-Checked Locking Pattern)
    public:

        static void SetMemResource(std::pmr::memory_resource* memResource)
        {
            m_MemResource = memResource;
        }

        static T* Get()
        {
            if (!m_Instance)
            {
                if (!m_Instance)
                {
                    std::scoped_lock lock(m_Mutex);
                    if (!m_Instance)
                    {
                        m_Instance = std::allocate_shared<T>(m_MemResource);
                    }
                }

                return m_Instance.get();

            }
            return m_Instance;
        }
         
    protected:
        ThreadSafeSingleton() = default;
    private:


        static SharedPtr<T> m_Instance;
        static std::mutex m_Mutex;
        static std::pmr::memory_resource* m_MemResource;
    };    
}
