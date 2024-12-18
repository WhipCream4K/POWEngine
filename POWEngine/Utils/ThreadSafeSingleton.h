#pragma once

namespace powe
{   
    template<typename T>
    class ThreadSafeSingleton
    {
        // Implements DCLP (Double-Checked Locking Pattern)
    public:

        static SharedPtr<T> Get()
        {
            if (!m_Instance)
            {
                if (!m_Instance)
                {
                    std::scoped_lock lock(m_Mutex);
                    if (!m_Instance)
                    {
                        m_Instance = std::make_shared<T>();
                    }
                }

                return m_Instance;

            }
            return m_Instance;
        }
        
        virtual ~ThreadSafeSingleton() = default;
         
    protected:

        ThreadSafeSingleton() = default;

        static inline SharedPtr<T> m_Instance{};

    private:

        static inline std::mutex m_Mutex{};
    };    

    
}
