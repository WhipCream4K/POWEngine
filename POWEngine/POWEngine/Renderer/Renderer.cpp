#include "pch.h"
#include "Renderer.h"
#include "RenderAPI.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"
#include "NullRenderer.h"
#include "POWEngine/Core/GameObject/GameObject.h"

powe::Renderer::Renderer()
    : m_RenderAPI(std::make_unique<NullRenderer>())
{
}

powe::Renderer::~Renderer() = default;

void powe::Renderer::Draw(const Window& window) const
{
    m_RenderAPI->DrawBufferOnWindow(window);
}

void powe::Renderer::RegisterSystem(const SharedPtr<RenderSystemBase>&)
{
    // if (system)
    // {
    //     if (std::ranges::find(m_RenderSystems, system) == m_RenderSystems.end())
    //         m_RenderSystems.emplace_back(system);
    // }
}

void powe::Renderer::RemoveSystem(RenderSystemBase* system)
{
    if (system)
    {
        auto removeRange = std::ranges::remove_if(m_RenderSystems,
                                                  [system](const SharedPtr<RenderSystemBase>& activeSystem)
                                                  {
                                                      return system == activeSystem.get();
                                                  });

        m_RenderSystems.erase(removeRange.begin(), removeRange.end());
    }
}

void powe::Renderer::UpdateSystem(const WorldEntity& worldEntity, const Window& renderWindow,
                                  const std::unordered_map<std::string, SharedPtr<Archetype>>& archetypePool)
{
    // for (const auto& system : m_PreRenderSystems)
    // {
    //     for (const auto& archetype : archetypePool | std::views::values) // since c++20
    //     {
    //         if (IsDigitExistInNumber(archetype->ComponentOffsets, system->GetKeys()))
    //         {
    //             system->InternalCreate(worldEntity, *archetype, *m_RenderAPI);
    //             m_RenderSystems.emplace_back(system);
    //         }
    //     }
    // }
    //
    // m_PreRenderSystems.clear();

    for (const auto& system : m_RenderSystems)
    {
        for (const auto& archetype : archetypePool | std::views::values) // since c++20
        {
            if (IsDigitExistInNumber(archetype->ComponentOffsets, system->GetKeys()))
                system->InternalDraw(worldEntity, renderWindow, *archetype, *m_RenderAPI);
        }
    }
}


void powe::Renderer::RegisterRenderAPI(OwnedPtr<RenderAPI>&& renderInst)
{
    m_RenderAPI = std::move(renderInst);
}
