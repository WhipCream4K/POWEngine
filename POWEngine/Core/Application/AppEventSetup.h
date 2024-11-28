#pragma once

namespace powe
{
    class AppEvent;
    class AppEventSetup
    {
    public:

        AppEventSetup(Vector<SharedPtr<AppEvent>>& appEvents);

        void Init(const SharedPtr<PMRResource>& resc);

        template<typename T>
        requires std::is_base_of_v<AppEvent, T>
        void Add()
        {
            
        }        


    private:

        RefWrap<Vector<SharedPtr<AppEvent>>> m_AppEvents;
    };
}