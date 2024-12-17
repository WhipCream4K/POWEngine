#include "pch.h"
#include "InputManager.h"

#include "InputSubsystem.h"


using namespace powe;

InputManager::InputManager(Game& gameModule)
    : m_GameModule(gameModule)
{
    m_InputSubsystem = InputSubsystem::Create(*this);

    AllocatorContext context{AllocatorScope::Game};
    auto* upStream{context.GetResource()};
    
    m_Observers = Vector<std::pair<Input::KeyBinding, Vector<WeakPtr<InputObserver>>>>{upStream};
}


void InputManager::AddKeyBinding(const SharedPtr<InputObserver>& observer,int key, Input::State state, uint8_t modifiers) noexcept
{
    const Input::KeyBinding binding{ key, modifiers, state };

    auto findBindings{ std::ranges::find_if(m_Observers, [binding](const auto& pair) { 
            return pair.first == binding;
    }) };

    if(findBindings == m_Observers.end())
    {
        AllocatorContext context{AllocatorScope::Game};
        auto* upStream{context.GetResource()};

        Vector<WeakPtr<InputObserver>> observers{upStream};
        observers.emplace_back(observer);
        
        m_Observers.emplace_back(binding, observers);
    }
    else
    {
        findBindings->second.emplace_back(observer);
    }
}

void InputManager::Update() noexcept
{
    m_InputSubsystem->Update(m_Observers);
}

const Vector<WeakPtr<InputObserver>>* InputManager::GetObservers(const Input::KeyBinding& binding) const noexcept
{
    auto findBindings{ std::ranges::find_if(m_Observers, [binding](const auto& pair) { 
            return pair.first == binding;
    }) };

    if(findBindings == m_Observers.end())
    {
        return nullptr;
    }

    return &findBindings->second;
}

void InputManager::RemoveObserver(const SharedPtr<InputObserver>& observer) noexcept
{
    for(auto& [binding, observers] : m_Observers)
    {
        auto findObserver{std::ranges::find_if(observers, [observer](const auto& weakObserver) {
            return weakObserver.lock() == observer;
        })};

        if(findObserver != observers.end())
        {
            observers.erase(findObserver);
            break;
        }
    }
}