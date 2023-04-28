#pragma once
#include "Action.h"
#include "GameTime.h"
#include "Vector.h"
#include "Datum.h"
#include "ConcreteFactory.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action for incrementing target 
	/// </summary>
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action);
	public: 
		/// <summary>
		/// Default Constructor
		/// </summary>
		ActionIncrement();
		
		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="">ActionIncrement to copy</param>
		ActionIncrement(const ActionIncrement&) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="">ActionIncrement to move</param>
		ActionIncrement(ActionIncrement&&) = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="">ActionIncrement to copy</param>
		/// <returns>Reference to  copy</returns>
		ActionIncrement& operator=(const ActionIncrement&) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="">ActionIncrement to move</param>
		/// <returns>Reference to ActionIncrement that was moved</returns>
		ActionIncrement& operator=(ActionIncrement&&) = default;

		/// <summary>
		/// Defaulted Destructor
		/// </summary>
		~ActionIncrement() = default;

		/// <summary>
		/// Invoking update will increment the targeted value
		/// </summary>
		/// <param name="gameTime"></param>
		void Update(const GameTime&) override;

		/// <summary>
		/// Target name that should be searched for to be incremented
		/// </summary>
		std::string Target;

		/// <summary>
		/// Number to increment by
		/// </summary>
		int Step = 0;

		/// <summary>
		/// Targets index if it is in array
		/// </summary>
		int TargetIndex = 0;

		/// <summary>
		/// Signatures of action increment
		/// </summary>
		/// <returns>Vectore of signatures</returns>
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};
	CONCRETE_FACTORY(ActionIncrement, FieaGameEngine::Scope);
}