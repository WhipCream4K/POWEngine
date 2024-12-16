#pragma once 

namespace powe 
{
    enum class BufferType
    {
        Vertex,
        Index,
        Uniform,
        Storage
    };

    class Buffer
    {
    public:  

        virtual ~Buffer() = default;

        virtual void* Map() noexcept = 0;
        virtual void Unmap() noexcept = 0;
        virtual void Update(const void* data, size_t size, size_t offset = 0) noexcept = 0;

        size_t GetSize() const noexcept { return m_Size; }
        BufferType GetType() const noexcept { return m_Type; }

    protected:

        Buffer(size_t size, BufferType type) : m_Size(size), m_Type(type) 
        {
        }

        size_t m_Size;
        BufferType m_Type;
    };
}