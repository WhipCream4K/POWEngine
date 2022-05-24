#include "pch.h"
#include "Transform2D.h"

#include "POWEngine/Core/GameObject/GameObject.h"

powe::Transform2D::Transform2D()
	: m_WorldScales(1.0f,1.0f)
	, m_LocalScales(1.0f,1.0f)
{
}


void powe::Transform2D::SetLocalPosition(const glm::vec2& position)
{
	m_LocalPosition = position;
	SetDirtyFlag(DirtyFlag::Position);
}

void powe::Transform2D::SetWorldPosition(const glm::vec2& position)
{
	m_WorldPosition = position;
}

void powe::Transform2D::SetWorldScale(const glm::vec2& scales)
{
	m_WorldScales = scales;
}

void powe::Transform2D::SetWorldRotation(float zRotation)
{
	m_WorldZRotation = zRotation;
}

void powe::Transform2D::SetLocalScale(const glm::vec2& scales)
{
	m_LocalScales = scales;
	SetDirtyFlag(DirtyFlag::Scale);
}


void powe::Transform2D::SetLocalRotation(float zRotation)
{
	m_LocalZRotation = zRotation;
	SetDirtyFlag(DirtyFlag::Rotation);
}

const glm::vec2& powe::Transform2D::GetWorldPosition()
{
	if (IsDirty(DirtyFlag::Position))
		UpdateData(DirtyFlag::Position);

	return m_WorldPosition;
}

const glm::vec2& powe::Transform2D::GetWorldScale()
{
	if (IsDirty(DirtyFlag::Scale))
		UpdateData(DirtyFlag::Scale);

	return m_WorldScales;
}

float powe::Transform2D::GetWorldRotation()
{
	if (IsDirty(DirtyFlag::Rotation))
		UpdateData(DirtyFlag::Rotation);

	return m_WorldZRotation;
}

float powe::Transform2D::GetLocalRotation() const
{
	return m_LocalZRotation;
}

const glm::vec2& powe::Transform2D::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::vec2& powe::Transform2D::GetLocalScale() const
{
	return m_LocalScales;
}


void powe::Transform2D::SetParent(const SharedPtr<GameObject>& gameObject, bool keepWorldPosition)
{
	if (gameObject)
	{
		Transform2D* parentTransform{ gameObject->GetComponent<Transform2D>() };
		if (!parentTransform)
			return;

		if (keepWorldPosition)
		{
			SetLocalPosition(GetLocalPosition() - parentTransform->GetWorldPosition());
			SetLocalRotation(parentTransform->GetWorldRotation() + GetLocalRotation());
			SetLocalScale(parentTransform->GetWorldScale() + GetLocalScale());
		}

		SetDirtyFlag(DirtyFlag::Position);
		SetDirtyFlag(DirtyFlag::Rotation);
		SetDirtyFlag(DirtyFlag::Scale);

		parentTransform->AddChild(m_Owner.lock());
	}
	else
	{
		SetLocalPosition(GetLocalPosition());
		SetLocalRotation(GetLocalRotation());
		SetLocalScale(GetLocalScale());
	}

	if (const auto currentParent{ m_ParentNode.lock() })
	{
		Transform2D* parentTransform{ currentParent->GetComponent<Transform2D>() };
		if (!parentTransform)
			return;

		parentTransform->RemoveChild(m_Owner.lock());
	}

	m_ParentNode = gameObject;
}

void powe::Transform2D::UpdateData(DirtyFlag flag)
{
	Transform2D* parentTransform{};
	if (const auto parent{ m_ParentNode.lock() })
	{
		parentTransform = parent->GetComponent<Transform2D>();
	}

	switch (flag)
	{
	case DirtyFlag::Position:

		if (parentTransform)
			m_WorldPosition = parentTransform->GetWorldPosition() + m_LocalPosition;
		else
			m_WorldPosition = m_LocalPosition;


		break;
	case DirtyFlag::Rotation:

		if (parentTransform)
			m_WorldZRotation = parentTransform->GetWorldRotation() + m_LocalZRotation;
		else
			m_WorldZRotation = m_LocalZRotation;

		break;
	case DirtyFlag::Scale:

		if (parentTransform)
			m_WorldScales = parentTransform->GetWorldScale() + m_LocalPosition;
		else
			m_WorldScales = m_LocalScales;

		break;
	default:;
	}

	m_DirtyFlag &= ~uint8_t(flag);
}

void powe::Transform2D::SetDirtyFlag(DirtyFlag flag)
{
	m_DirtyFlag |= uint8_t(flag);

	for (auto it = m_ChildrenNode.begin(); it != m_ChildrenNode.end();)
	{
		if(const auto child{it->lock()})
		{
			Transform2D* transform2D{ child->GetComponent<Transform2D>() };
			transform2D->SetDirtyFlag(flag);
			++it;
		}
		else
		{
			it = m_ChildrenNode.erase(it);
		}
	}
}

void powe::Transform2D::AddChild(const SharedPtr<GameObject>& gameObject)
{
	if (std::ranges::find_if(m_ChildrenNode, [&gameObject](const WeakPtr<GameObject>& item)
		{
			return item.lock() == gameObject;
		}) == m_ChildrenNode.end())
	{
		m_ChildrenNode.emplace_back(gameObject);
	}
}

void powe::Transform2D::RemoveChild(const SharedPtr<GameObject>& gameObject)
{
	const auto removeItr{ std::ranges::find_if(m_ChildrenNode,[&gameObject](const WeakPtr<GameObject>& item)
	{
		return item.lock() == gameObject;
	}) };

	m_ChildrenNode.erase(removeItr);
}

bool powe::Transform2D::IsDirty(DirtyFlag flag) const
{
	return m_DirtyFlag & uint8_t(flag);
}
