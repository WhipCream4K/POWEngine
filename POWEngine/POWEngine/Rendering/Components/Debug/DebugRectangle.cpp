#include "pch.h"
#include "DebugRectangle.h"

#ifdef USE_SFML_RENDERER
#include "SFML/SFMLDebugRectangle.h"
using RectangleType = powe::SFMLDebugRectangle;
#endif

powe::DebugRectangle::DebugRectangle()
	: m_RectangleImpl(std::make_unique<RectangleType>())
{
}

powe::DebugRectangle::DebugRectangle(const SharedPtr<GameObject>& owner)
	: m_RectangleImpl(std::make_unique<RectangleType>(owner))
{
}

powe::DebugRectangle::DebugRectangle(const SharedPtr<GameObject>& owner, const glm::fvec2& size)
	: m_RectangleImpl(std::make_unique<RectangleType>(owner,size))
{
}

powe::DebugRectangle::DebugRectangle(DebugRectangle&&) noexcept = default;

powe::DebugRectangle& powe::DebugRectangle::operator=(DebugRectangle&&) noexcept = default;

powe::DebugRectangle::~DebugRectangle() = default;

void powe::DebugRectangle::OnDestroy(WorldEntity& worldEntity, GameObjectID gameObjectId)
{
	m_RectangleImpl->Destroy(worldEntity, gameObjectId);
}

void powe::DebugRectangle::SetSize(const glm::fvec2& size) const
{
	m_RectangleImpl->SetSize(size);
}

const glm::fvec2& powe::DebugRectangle::GetRectSize() const
{
	return m_RectangleImpl->GetSize();
}

void powe::DebugRectangle::SetFillColor(const glm::uvec4& color) const
{
	m_RectangleImpl->SetFillColor(color);
}

const glm::uvec4& powe::DebugRectangle::GetOutlineColor() const
{
	return m_RectangleImpl->GetOutlineColor();
}

void powe::DebugRectangle::SetOutlineColor(const glm::uvec4& color) const
{
	m_RectangleImpl->SetOutlineColor(color);
}

const glm::uvec4& powe::DebugRectangle::GetFillColor() const
{
	return m_RectangleImpl->GetFillColor();
}

void powe::DebugRectangle::SetOrigin(const glm::fvec2& origin) const
{
	m_RectangleImpl->SetOrigin(origin);
}

const glm::fvec2& powe::DebugRectangle::GetOrigin() const
{
	return m_RectangleImpl->GetOrigin();
}

void powe::DebugRectangle::SetOutlineThickness(float thickness) const
{
	m_RectangleImpl->SetOutlineThickness(thickness);
}


