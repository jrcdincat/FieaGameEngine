#include "pch.h"
#include "GameState.h"

namespace FieaGameEngine
{
	GameState::GameState(GameObject* gameObject) : _root{ gameObject }
	{

	}

	void GameState::Update()
	{
		_root->Update(_gameTime);
		_eventQueue.Update(_gameTime);
	}

	EventQueue* GameState::GetEventQueue()
	{
		return &_eventQueue;
	}

	GameTime& GameState::GetGameTime()
	{
		return _gameTime;
	}

	GameState* GameState::Instance()
	{
		return _instance;
	}

	void GameState::CreateInstance(GameObject& gameObject)
	{
		assert(_instance == nullptr);
		_instance = new GameState(&gameObject);
	}

	void GameState::DestroyInstance()
	{
		delete _instance;
	}

}