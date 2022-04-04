#include "pch.h"
#include "Transform2D.h"

void powe::Transform2D::SetPosition(const glm::vec2& position)
{
	m_LocalPosition = position;
}

void powe::Transform2D::SetScales(const glm::vec2& scales)
{
	m_LocalScales = scales;
}

void powe::Transform2D::SetRotation(float zRotation)
{
	m_LocalZRotation = zRotation;
}

void powe::Transform2D::SetDepth(float depth)
{
	m_Depth = depth;
}

const glm::vec2& powe::Transform2D::GetPosition()
{
	return m_LocalPosition;
}

const glm::vec2& powe::Transform2D::GetScales()
{
	return m_LocalScales;
}

float powe::Transform2D::GetDepth()
{
	return m_Depth;
}

float powe::Transform2D::GetRotation()
{
	return m_LocalZRotation;
}
