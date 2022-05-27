#include "pch.h"
#include "Renderer.h"
#include "RenderAPI.h"
#include "POWEngine/Rendering/System/RenderSystemBase.h"
#include "NullRenderer.h"

powe::Renderer::Renderer()
	: m_RenderAPI(std::make_unique<NullRenderer>())
{
}

powe::Renderer::~Renderer() = default;

void powe::Renderer::Draw(const Window& window) const
{
	m_RenderAPI->DrawBufferOnWindow(window);
}

void powe::Renderer::RegisterSystem(const SharedPtr<RenderSystemBase>& system)
{
	if(system)
	{
		if (std::ranges::find(m_RenderSystems, system) == m_RenderSystems.end())
			m_RenderSystems.emplace_back(system);
	}
}

void powe::Renderer::RemoveSystem(const SharedPtr<RenderSystemBase>& system)
{
	if(system)
	{
		m_RenderSystems.erase(std::ranges::remove(m_RenderSystems, system).begin(), m_RenderSystems.end());
	}
}

void powe::Renderer::UpdateSystem(const WorldEntity& worldEntity,
	const std::unordered_map<std::string, SharedPtr<Archetype>>& archetypePool) const
{
	for (const auto& system : m_RenderSystems)
	{
		for (const auto& archetype : archetypePool | std::views::values) // since c++20
		{
			if (IsDigitExistInNumber(archetype->ComponentOffsets, system->GetKeys()))
				system->InternalDraw(worldEntity,*archetype, *m_RenderAPI);
		}
	}
}

void powe::Renderer::RegisterRenderAPI(OwnedPtr<RenderAPI>&& renderInst)
{
	m_RenderAPI = std::move(renderInst);
}
