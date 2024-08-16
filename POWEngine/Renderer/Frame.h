#pragma once

namespace powe
{
    class Frame
    {
    public:

        Frame(PMRResource* memResource = DefaultAllocator::Engine);

        void Bind();
        void Clear();
        
    private:
        
        void* FrameHandle;
    };    
}

