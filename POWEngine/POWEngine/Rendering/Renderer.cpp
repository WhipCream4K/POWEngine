#include "pch.h"
#include "Renderer.h"

#include "RendererImpl.h"
#include "POWEngine/Window/Window.h"

void powe::Renderer::RegisterSystem(const SharedPtr<RenderSystem>& system)
{
	if(system)
	{
		if (std::ranges::find(m_RenderSystems, system) == m_RenderSystems.end())
			m_RenderSystems.emplace_back(system);
	}
}

void powe::Renderer::RemoveSystem(const SharedPtr<RenderSystem>& system)
{
	if(system)
	{
		m_RenderSystems.erase(std::ranges::remove(m_RenderSystems, system).begin(), m_RenderSystems.end());
	}
}
