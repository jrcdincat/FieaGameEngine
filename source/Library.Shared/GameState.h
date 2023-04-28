#pragma once
#include "GameTime.h"
#include "EventQueue.h"
#include "GameObject.h"

namespace FieaGameEngine
{
	class GameState final
	{
	public: 
		/// <summary>
		/// Defaulted constructor for Game State
		/// </summary>
		GameState() = default;

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="gameObject">Root game object</param>
		GameState(GameObject* gameObject);

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="other"> - GameState to copy</param>
		GameState(const GameState& other) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="other">GameState to move</param>
		/// <returns> - Moved GameState</returns>
		GameState(GameState&& other) noexcept = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="rhs">GameState to copy</param>
		/// <returns> - Reference to GameState copy</returns>
		GameState& operator=(const GameState& rhs) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="rhs">GameState to move</param>
		/// <returns> - Reference to moved GameState</returns>
		GameState& operator=(GameState&& rhs) noexcept = default;

		/// <summary>
		/// Defaulted GameState destructor
		/// </summary>
		~GameState() = default;

		/// <summary>
		/// Update currently calls event queue update
		/// </summary>
		void Update();

		/// <summary>
		/// Returns the event queue
		/// </summary>
		/// <returns>Event Queue</returns>
		EventQueue* GetEventQueue();

		/// <summary>
		/// Returns game time
		/// </summary>
		/// <returns>Game time reference</returns>
		GameTime& GetGameTime();

		/// <summary>
		/// Gets the static type manager instance
		/// </summary>
		/// <returns>static type manager instance</returns>
		static GameState* Instance();

		/// <summary>
		/// Instantiate the static type manager instance.
		/// </summary>
		static void CreateInstance(GameObject& gameObject);

		/// <summary>
		/// Delete the static type manager instance.
		/// </summary>
		static void DestroyInstance();


	private: 
		/// <summary>
		/// Game time
		/// </summary>	
		GameTime _gameTime;

		/// <summary>
		/// Event Queue
		/// </summary>
		EventQueue _eventQueue;

		/// <summary>
		/// Root game object
		/// </summary>
		GameObject* _root;

		/// <summary>
		/// Static type manager instance.
		/// </summary>
		inline static GameState* _instance{ nullptr };
	};
}