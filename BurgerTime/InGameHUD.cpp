#include "InGameHUD.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Text/TextComponent.h"
#include "AssetManager.h"
#include "BurgerTimeComponents.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "POWEngine/Rendering/Resources/Texture/Texture.h"
#include "BurgerTimeComponents.h"
#include "PepperNumberDisplay.h"
#include "ScoreListener.h"

std::vector<SharedPtr<powe::GameObject>> InGameHUD::Create(powe::WorldEntity& worldEntity, const IngameHUDDesc& desc)
{
	using namespace powe;

	std::vector<SharedPtr<GameObject>> outGameObject{};

	const glm::fvec2 midScreen{ 640.0f,360.0f };
	const glm::fvec2 levelRect{ 208.0f,208.0f };

	glm::fvec2 banner1Pos{ midScreen - ((levelRect / 2.0f)) * burger::SpriteScale };
	const float offset{ 30.0f * burger::SpriteScale.y };
	banner1Pos.y -= offset;

	const glm::fvec2 score1Pos{ banner1Pos + 30.f };

	const auto banner1Up{ CreateTextDisplay(worldEntity,banner1Pos,30,"1Up",{255,0,0,255}) };
	outGameObject.emplace_back(banner1Up);

	// Health Pos
	// Player 1 to the left of the screen
	glm::fvec2 health1Pos{ midScreen };
	health1Pos.x -= ((levelRect.x / 2.0f) + 30.0f) * burger::SpriteScale.x;
	health1Pos.y += (levelRect.y / 2.0f) * burger::SpriteScale.y;
	const auto healthDisplay{ CreateHealthDisplay(worldEntity, health1Pos) };

	DynamicSceneData* dynamicSceneData = worldEntity.FindUniqueComponent<DynamicSceneData>();
	if (dynamicSceneData)
	{
		// un-safe but it'll do

	}

	//const auto displayManager{ std::make_shared<GameObject>(worldEntity) };
	//DisplayManager* displayManagerComp = displayManager->AddComponent(DisplayManager{});

	//displayManagerComp->banner[0] = banner1Up;
	//displayManagerComp->score[0] = healthDisplay;

	if (desc.playMode == PlayMode::Coop)
	{

	}

	return outGameObject;
}

SharedPtr<powe::GameObject> InGameHUD::CreateDisplayManager(powe::WorldEntity& worldEntity, const IngameHUDDesc& desc)
{
	using namespace powe;

	auto gameObject{ std::make_shared<GameObject>(worldEntity) };
	DisplayManager* displayManager = gameObject->AddComponent(DisplayManager{});

	displayManager->ScoreListener = std::make_shared<ScoreListener>(gameObject->GetID());
	displayManager->PepperNumberDisplay = std::make_shared<PepperNumberDisplay>(gameObject->GetID());

	if (desc.playMode == PlayMode::Coop)
	{
		CreateHUDElements(worldEntity, gameObject->GetID(), displayManager, desc, -1); // player 1
		CreateHUDElements(worldEntity, gameObject->GetID(), displayManager, desc, 1); // player 2
	}
	else
	{
		CreateHUDElements(worldEntity, gameObject->GetID(), displayManager, desc, -1); // player 1
	}


	return gameObject;
}

std::vector<SharedPtr<powe::GameObject>> InGameHUD::CreateSubObject(powe::WorldEntity& worldEntity, const IngameHUDDesc&)
{
	using namespace powe;

	std::vector<SharedPtr<GameObject>> outGameObject{};

	const auto& assetManager{ Instance<AssetManager>() };
	const auto& mainFont{ assetManager->GetAsset<Font>(burger::MainFont) };

	SharedPtr<powe::GameObject> playerBanner{ std::make_shared<GameObject>(worldEntity) };
	outGameObject.emplace_back(playerBanner);

	const glm::fvec2 levelRect{ 208.0f,208.0f };
	const glm::fvec2 midscreen{ 640.0f,360.0f };
	glm::fvec2 position{ midscreen - (levelRect / 2.0f * burger::SpriteScale) };
	const float offset{ 30.0f * burger::SpriteScale.y };

	position.y -= offset;

	Transform2D* transform2D = playerBanner->AddComponent(Transform2D{ playerBanner });
	transform2D->SetWorldPosition(position);

	TextComponent* textComponent = playerBanner->AddComponent(TextComponent{ 30,playerBanner }, ComponentFlag::Sparse);
	textComponent->SetText("1UP");
	textComponent->SetFont(mainFont);
	textComponent->SetFillColor({ 255,0,0,255 });

	const auto playerScoreDisplay{ std::make_shared<GameObject>(worldEntity) };
	outGameObject.emplace_back(playerScoreDisplay);

	transform2D = playerScoreDisplay->AddComponent(Transform2D{ playerScoreDisplay });
	position.y += 30.0f;
	transform2D->SetWorldPosition(position);

	textComponent = playerScoreDisplay->AddComponent(TextComponent{ 30,playerScoreDisplay }, ComponentFlag::Sparse);
	textComponent->SetText("0");
	textComponent->SetFont(mainFont);

	return outGameObject;
}

SharedPtr<powe::GameObject> InGameHUD::CreateHealthDisplay(powe::WorldEntity& worldEntity, const glm::fvec2& position)
{
	using namespace powe;

	const auto& assetManager{ Instance<AssetManager>() };
	const auto& healthTexture{ assetManager->GetAsset<Texture>(burger::HealthSprite) };

	auto gameObject{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* transform2D = gameObject->AddComponent(Transform2D{ gameObject });
	transform2D->SetWorldPosition(position);
	transform2D->SetWorldScale(burger::SpriteScale);

	SpriteComponent* spriteComponent = gameObject->AddComponent(SpriteComponent{ gameObject }, ComponentFlag::Sparse);
	spriteComponent->SetTexture(*healthTexture);
	const float spriteWidth{ 8.0f };
	spriteComponent->SetRect({ 0.0f,0.0f,spriteWidth,spriteWidth * 3.0f });
	spriteComponent->SetRenderOrder(burger::RenderOrder::UI);

	return gameObject;
}

SharedPtr<powe::GameObject> InGameHUD::CreateTextDisplay(powe::WorldEntity& worldEntity,
	const glm::fvec2& position,
	int textSize,
	const std::string& text,
	const glm::uvec4& color)
{
	using namespace powe;

	const auto& assetManager{ Instance<AssetManager>() };
	const auto& mainFont{ assetManager->GetAsset<Font>(burger::MainFont) };

	auto gameobject{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* transform2D = gameobject->AddComponent(Transform2D{ gameobject });
	transform2D->SetWorldPosition(position);

	TextComponent* textComponent{ gameobject->AddComponent(TextComponent{textSize,gameobject},ComponentFlag::Sparse) };
	textComponent->SetText(text);
	textComponent->SetFillColor(color);
	textComponent->SetFont(mainFont);

	return gameobject;
}

void InGameHUD::CreateHUDElements(powe::WorldEntity& worldEntity, powe::GameObjectID, DisplayManager* displayManager, const IngameHUDDesc& desc, int textDir)
{
	const glm::fvec2 midScreen{ 640.0f,360.0f };
	const glm::fvec2 levelRect{ 208.0f,208.0f };

	const int playerIdx{ textDir < 0 ? 0 : 1 };

	glm::fvec2 banner1Pos{ midScreen - ((levelRect / 2.0f)) * burger::SpriteScale };
	banner1Pos.x = midScreen.x + (levelRect.x / 2.0f * float(textDir)) * burger::SpriteScale.x;
	const float offset{ 30.0f * burger::SpriteScale.y };
	banner1Pos.y -= offset;

	// 1 Up banner
	const std::string bannerText{ textDir < 0 ? "1Up" : "2Up" };
	const glm::uvec4 bannerColor{ textDir < 0 ? glm::uvec4{255,0,0,255} : glm::uvec4{0,0,255,255} };
	const auto banner1Up{ CreateTextDisplay(worldEntity,banner1Pos,30,bannerText,bannerColor) };
	displayManager->banner[playerIdx] = banner1Up;

	// Score
	glm::fvec2 score1Pos{ banner1Pos };
	score1Pos.y += 30.0f;
	const auto score1{ CreateTextDisplay(worldEntity,score1Pos,30,"0",{255,255,255,255}) };
	displayManager->score[playerIdx] = score1;
	displayManager->CurrentScore[playerIdx] = 0;


	// Health Pos
	// Player 1 to the left of the screen
	glm::fvec2 health1Pos{ midScreen };
	health1Pos.x += ((levelRect.x / 2.0f) * burger::SpriteScale.x * float(textDir));
	health1Pos.x += float(textDir) * 30.0f * burger::SpriteScale.x;
	health1Pos.y -= (levelRect.y / 2.0f - 30.0f) * burger::SpriteScale.y;
	const auto healthDisplay{ CreateHealthDisplay(worldEntity, health1Pos) };
	displayManager->healthDisplay[playerIdx] = healthDisplay;
	displayManager->CurrentLives[playerIdx] = desc.startLives;

	// Pepper display
	glm::fvec2 pepperTextPos{ midScreen };
	pepperTextPos.x += ((levelRect.x / 2.0f) * burger::SpriteScale.x * float(textDir));
	pepperTextPos.x += (60.0f * float(textDir)) * burger::SpriteScale.x;
	pepperTextPos.y = health1Pos.y;
	pepperTextPos.y -= 30.0f * burger::SpriteScale.y;

	const auto pepperText{ CreateTextDisplay(worldEntity,pepperTextPos,20,"Pepper",{0,255,0,255}) };
	displayManager->pepperText[playerIdx] = pepperText;

	glm::fvec2 pepperNumPos{ pepperTextPos };
	pepperNumPos.x += 35.0f * burger::SpriteScale.x * float(textDir) * -1.0f;
	const auto pepNum{ CreateTextDisplay(worldEntity,pepperNumPos,20,std::to_string(desc.startPepper),{255,255,255,255}) };
	displayManager->pepperNumber[playerIdx] = pepNum;

	displayManager->CurrentPepper[playerIdx] = desc.startPepper;
}
