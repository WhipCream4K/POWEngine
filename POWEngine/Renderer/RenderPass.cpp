#include "pch.h"
#include "RenderPass.h"

powe::RenderPass::RenderPass(std::string_view passName, RenderFunction func, Viewport::Flag requireFlag)
    : m_Name(passName)
      , m_ExecuteFunction(std::move(func))
      , m_RequireFlag(requireFlag)
{
}

void powe::RenderPass::AddDependency(std::string_view passName)
{
    m_Dependencies.emplace_back(passName);
}

void powe::RenderPass::Execute(RenderContext& ctx) const
{
    m_ExecuteFunction(ctx);
}
