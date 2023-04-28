#pragma once
#include "Attributed.h"
#include "ConcreteFactory.h"
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action Event
	/// </summary>
	class ActionEvent final : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action);
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		ActionEvent();

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other"> - to Copy</param>
		ActionEvent(const ActionEvent& other) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other"> to move</param>
		ActionEvent(ActionEvent&& other) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs"> to copy</param>
		/// <returns> copied </returns>
		ActionEvent& operator=(const ActionEvent& rhs) = default;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns> - to move </returns>
		ActionEvent& operator=(ActionEvent&& rhs) = default;

		/// <summary>
		/// Destructor
		/// </summary>
		~ActionEvent() override = default;

		/// <summary>
		/// Updates
		/// </summary>
		/// <param name="gameTime"> Time </param>
		void Update(const GameTime& gameTime) override;

		/// <summary>
		/// Set subtype
		/// </summary>
		/// <param name="subType"> - type</param>
		void SetSubtype(const std::string& subType);

		/// <summary>
		/// Delay
		/// </summary>
		/// <returns>Time in ms</returns>
		const uint32_t GetDelay();

		/// <summary>
		/// Signatures of action list
		/// </summary>
		/// <returns>Vector of signatures</returns>
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	private: 

		/// <summary>
		/// Subtype
		/// </summary>
		std::string SubType;

		/// <summary>
		/// Delay time ms
		/// </summary>
		uint32_t Delay = 0;
	};

	CONCRETE_FACTORY(ActionEvent, FieaGameEngine::Scope);
}