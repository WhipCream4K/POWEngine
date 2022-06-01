#include "PlayerInputSystem.h"

#include "BurgerTimeComponents.h"
#include "POWEngine/Core/Components/AudioComponent.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Logger/LoggerUtils.h"

PlayerInputSystem::PlayerInputSystem()
{
	using namespace powe;

	DEFINE_SYSTEM_KEY(Transform2D);
}

void PlayerInputSystem::OnUpdate(float deltaTime, powe::GameObjectID )
{
	using  namespace powe;

	if (m_Timer < 2.0f)
		m_Timer += deltaTime;
	else
	{
		GetWorld()->RemoveGameObject(1);
	}
	//const float axis = GetWorld()->GetInputSettings().GetInputAxis("Horizontal");

	//POWLOGNORMAL(std::to_string(axis));

	//constexpr float speed{ 20.0f };
	//if(GetWorld()->GetInputSettings().GetInputEvent("Horizontal") == InputEvent::IE_Down)
	//{
	//	Transform2D* transform{ GetComponent<Transform2D>() };
	//	glm::vec2 pos{ transform->GetWorldPosition() };
	//	pos += glm::vec2{1.0f,0.0f} * speed;
	//	transform->SetLocalPosition(pos);
	//}
}

void PlayerInputSystem::OnCreate(powe::GameObjectID )
{
	//using namespace powe;

	//AudioComponent* audio{ GetComponent<AudioComponent>() };

	//SoundInfo info{};
	//info.volume = 1.0f;
	//info.pitch = 0.5f;
	//info.isLooped = false;

	//audio->Play(info);
}
