#include "pch.h"
#include "Transform2D.h"

#include "POWEngine/Core/GameObject/GameObject.h"

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

void powe::Transform2D::SetParent(const SharedPtr<GameObject>& gameObject)
{
	if(gameObject)
	{
		if(const auto parent{m_ParentNode.lock()})
		{
			if(Transform2D* parentTransform{ parent->GetComponent<Transform2D>() })
			{
				parentTransform->SetParent(nullptr);
			}
		}
	}
	else
	{
		
	}
}
