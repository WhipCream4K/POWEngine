#pragma once

namespace powe
{
    class UsePMR
    {
    public:

        virtual ~UsePMR() = default;

        virtual void OnAllocate(const SharedPtr<PMRResource>&) {}

        SharedPtr<PMRResource> GetResource() const noexcept { return m_MemResource; }

    protected:

        UsePMR(const SharedPtr<PMRResource>& upStream = nullptr);

    private:

        SharedPtr<PMRResource> m_MemResource;

    };
}