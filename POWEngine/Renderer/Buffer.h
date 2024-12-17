#pragma once 

namespace powe 
{


    class Buffer
    {
    public:  

        enum class Type
        {
            Vertex,
            Index,
            Uniform,
            Storage
        };

        enum Usage : uint8_t
        {
            Stream  = 0,
            Static  = 1 << 0,
            Dynamic = 1 << 1,
            Draw    = 1 << 2,
            Read    = 1 << 3,
            Copy    = 1 << 4
        };

        virtual ~Buffer() = default;

        virtual void* Map() noexcept = 0;
        virtual void Unmap() noexcept = 0;
        virtual void Update(const void* data, size_t size, size_t offset = 0) noexcept = 0;

        size_t GetSize() const noexcept { return m_Size; }
        Type GetType() const noexcept { return m_Type; }

    protected:

        constexpr Buffer(size_t size, Type type, Usage usage) 
            : m_Size(size)
            , m_Type(type) 
            , m_Usage(usage)
        {
        }

        size_t m_Size;
        Type m_Type;
        Usage m_Usage;
    };
}