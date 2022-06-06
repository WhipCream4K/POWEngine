#pragma once

#include <poweComponent.h>
#include "PlayModeObserver.h"
#include "PlayModeSubject.h"
#include "WinConditionListener.h"
#include "StaticVariables.h"
#include "POWEngine/Core/WorldEntity/PipelineLayer.h"

class ScoreListener;
class OnIngredientStepped;
class SceneFactory;

namespace powe
{
	class GameObject;
	class SystemBase;
}

struct AnimationComponent : powe::Component<AnimationComponent>
{
	AnimationComponent() = default;

	AnimationComponent(int nbSprites, float timeSpan)
		: spritePerSec(timeSpan / float(nbSprites))
		, totalSprite(nbSprites)
	{
	}

	glm::fvec4 currentSpriteRect{};
	float spritePerSec{};
	float totalTime{};
	int totalSprite{};
	int currentSprite{};
};

struct Speed : powe::Component<Speed>
{
	Speed() = default;

	Speed(float inSpeed)
		: speed(inSpeed)
	{
	}

	float speed{};
	float climbSpeed{};
};

struct MenuData : powe::Component<MenuData>
{
	MenuData()
		: playModeSubject(std::make_shared<PlayModeSubject>())
	{
	}

	SharedPtr<PlayModeSubject> playModeSubject{};
};

enum class IntervalPlayState
{
	Maploaded,
	LevelWin,
	ShouldChangeLevel,
	Playing
};

class GameState;
class SceneFactory;
class WinConditionListener;
struct DynamicSceneData : powe::Component<DynamicSceneData>
{
	DynamicSceneData() = default;

	DynamicSceneData(powe::GameObjectID ownerID)
		: playModeObserver(std::make_shared<PlayModeObserver>(ownerID))
	{
	}

	int currentLevel{};
	int maxLevel{};

	float timeBeforeGameStart{};
	float gameStartTimeCounter{};

	PlayMode currentPlayMode{ PlayMode::SinglePlayer };
	SharedPtr<PlayModeObserver> playModeObserver{};
	SharedPtr<WinConditionListener> OnGameWin{};
	SharedPtr<GameState> currentGameState{};
	SharedPtr<powe::GameObject> DisplayManager{};

	// Any system that stop any entities from moving
	std::unordered_map<PipelineLayer,std::vector<SharedPtr<powe::SystemBase>>> blockingSystem{};

	SharedPtr<SceneFactory> currentScene{};
	IntervalPlayState intervalPlayState{};

	//bool shouldStartEndLevel{};
	//bool shouldChangeLevel{};
};

struct StepHandler : powe::Component<StepHandler>
{
	SharedPtr<OnIngredientStepped> OnIngredientStepped{};
	powe::GameObjectID stepHandlerID{};
	bool hasAlreadySteppedOn{};
};

struct SceneReference : powe::Component<SceneReference>
{
	powe::GameObjectID sceneID{};
};

class OnIngredientServing;
struct PlateComponent : powe::Component<PlateComponent>
{
	glm::fvec2 realIngredientSize{};
	SharedPtr<OnIngredientServing> OnServing{};
	int ingredientStackCount{};
	int maxStackCount{};
};

enum class MoveDir
{
	None,
	Left,
	Right,
	Up,
	Down
};

struct MovementDetails
{
	glm::fvec2 oldPos{};
	glm::fvec2 futurePos{};
	MoveDir currentMovementDir{ MoveDir::None };
};

struct LimitPlayArea : powe::Component<LimitPlayArea>
{
	glm::fvec4 rect{};
};

class OnPlayerThrowPepper;
class OnPlayerDead;
struct PlayerTag : powe::Component<PlayerTag>
{
	SharedPtr<OnPlayerThrowPepper> OnPlayerThrowPepper{};
	SharedPtr<OnPlayerDead> OnPlayerDead{};
	int playerIndex{};
	float respawnCounter{};
	bool isDead{};
};

struct DelayedMovement : powe::Component<DelayedMovement>
{
	float timeToReachNextSplit{};
	float timeCounter{};
	bool isMoving{};
};

struct FallingComponent : powe::Component<FallingComponent>
{
	FallingComponent() = default;

	FallingComponent(float inSpeed)
		: speed(inSpeed)
	{
	}

	float speed{};
};

struct CanWalkOnTile : powe::Component<CanWalkOnTile>
{
	CanWalkOnTile() = default;

	CanWalkOnTile(int levelIdx)
		: currentLevel(levelIdx)
	{
	}

	MovementDetails movementDetails{};
	int currentLevel{};
	int currentCol{};
	int currentRow{};
};

struct ScoreComponent : powe::Component<ScoreComponent>
{ 
	SharedPtr<powe::GameObject> owner{};
	int currentScore{};
};

class PepperNumberDisplay;
class HUDDisplay;
struct DisplayManager : powe::Component<DisplayManager>
{
	SharedPtr<powe::GameObject> score[int(PlayMode::Count)]{};
	SharedPtr<powe::GameObject> banner[int(PlayMode::Count)]{};
	SharedPtr<powe::GameObject> healthDisplay[int(PlayMode::Count)]{};
	SharedPtr<powe::GameObject> pepperText[int(PlayMode::Count)]{};
	SharedPtr<powe::GameObject> pepperNumber[int(PlayMode::Count)]{};

	SharedPtr<ScoreListener> ScoreListener{};
	SharedPtr<PepperNumberDisplay> PepperNumberDisplay{};
	SharedPtr<HUDDisplay> LivesDisplay{};

	int CurrentScore[int(PlayMode::Count)]{};
	int CurrentPepper[int(PlayMode::Count)]{};
	int CurrentLives[int(PlayMode::Count)]{};
};

struct EnemyTag : powe::Component<EnemyTag>
{
	EnemyType enemyType{};
	float stunnedRecoverCounter{};
	bool isStunned{};
};
