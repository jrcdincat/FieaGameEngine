#pragma once
#include "Attributed.h"
#include "GameState.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Event Message Attributed
	/// </summary>
	class EventMessageAttributed final: public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);
	public:		
		/// <summary>
		/// Constructor
		/// </summary>
		EventMessageAttributed();

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="other"> - EventMessageAttributed to copy</param>
		EventMessageAttributed(const EventMessageAttributed& other) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="other">EventMessageAttributed to move</param>
		/// <returns> - Moved EventMessageAttributed</returns>
		EventMessageAttributed(EventMessageAttributed&& other) noexcept = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="rhs">EventMessageAttributed to copy</param>
		/// <returns> - Reference to EventMessageAttributed copy</returns>
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="rhs">EventMessageAttributed to move</param>
		/// <returns> - Reference to moved EventMessageAttributed</returns>
		EventMessageAttributed& operator=(EventMessageAttributed&& rhs) noexcept = default;

		/// <summary>
		/// Defaulted EventMessageAttributed destructor
		/// </summary>
		~EventMessageAttributed() = default;

		/// <summary>
		/// Set game state
		/// </summary>
		/// <param name="gameState"> game state</param>
		void SetGameState(GameState& gameState);

		/// <summary>
		/// Get subtype
		/// </summary>
		/// <returns>type</returns>
		const std::string& GetSubType() const;

		/// <summary>
		/// Set subtype
		/// </summary>
		/// <param name="subType">type</param>
		void SetSubType(const std::string& subType);

		/// <summary>
		/// Signatures
		/// </summary>
		/// <returns>vector of signatures</returns>
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	private : 
		/// <summary>
		/// Root world
		/// </summary>
		GameState* _root = nullptr;

		/// <summary>
		/// Subtype
		/// </summary>
		std::string _subType;
	};
}