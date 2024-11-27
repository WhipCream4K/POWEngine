#include "pch.h"
#include "RenderPass.h"

powe::RenderPass::RenderPass(std::string_view passName)
    : m_Name(passName)
{
}

void powe::RenderPass::AddDependency(std::string_view passName)
{
    m_Dependencies.emplace_back(passName);
}
