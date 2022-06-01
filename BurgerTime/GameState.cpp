#include "GameState.h"

#include "PlayState.h"
#include "MenuState.h"
#include "GameStart.h"

SharedPtr<GameState> GameState::PlayState{ std::make_shared<class PlayState>() };
SharedPtr<GameState> GameState::MenuState{ std::make_shared<class MenuState>() };
SharedPtr<GameState> GameState::GameStart{ std::make_shared<class GameStart>() };