#include "MenuScene.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/Components/InputComponent.h"
#include "MenuCommands.h"
#include "BurgerTimeComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"

#include "AssetManager.h"
#include "POWEngine/Rendering/Components/Text/TextComponent.h"

MenuScene::MenuScene(powe::GameObjectID sceneDataID)
	: m_SceneDataID(sceneDataID)
{
}

void MenuScene::LoadScene(powe::WorldEntity& worldEntity)
{
	using namespace powe;

	DynamicSceneData* dynamicSceneData{ worldEntity.FindUniqueComponent<DynamicSceneData>() };
	if (!dynamicSceneData)
		return;

	const auto controller{ std::make_shared<GameObject>(worldEntity) };
	AddGameObject(controller);

	Transform2D* transform2D = controller->AddComponent(Transform2D{ controller });
	transform2D->SetWorldPosition({ 640.0f,360.0f });
	transform2D->SetWorldScale({ 0.25f,0.25f });

	SpriteComponent* spriteComp = controller->AddComponent(SpriteComponent{ controller }, ComponentFlag::Sparse);
	spriteComp->SetTexture(*Instance<AssetManager>()->GetAsset<Texture>(burger::MenuPointer));

	MenuData* menuData = controller->AddComponent(MenuData{});
	// add observer to subject
	menuData->playModeSubject->Attach(dynamicSceneData->playModeObserver.get());

	InputComponent* inputComponent = controller->AddComponent(InputComponent{});
	inputComponent->AddAxisCommand("MenuVertical", std::make_shared<MenuCommands>());

	const auto banner1{ std::make_shared<GameObject>(worldEntity) };
	AddGameObject(banner1);
	banner1->AddComponent(TextComponent{ 30,banner1 }, ComponentFlag::Sparse);

	const auto banner2{ std::make_shared<GameObject>(worldEntity) };
	AddGameObject(banner2);
}
