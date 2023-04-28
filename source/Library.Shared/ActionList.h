#pragma once
#include "Action.h"
#include "Datum.h"
#include "GameTime.h"
#include "Vector.h"
#include <string>
#include "ConcreteFactory.h"

namespace FieaGameEngine
{
	/// <summary>
	/// ActionList is a concrete class that contains a prescribed attribute of Actions
	/// </summary>
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);
	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		ActionList();

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="">ActionList to copy</param>
		ActionList(const ActionList&) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="">ActionList to move</param>
		ActionList(ActionList&&) noexcept = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="">ActionList to copy</param>
		/// <returns>Reference to ActionList copy</returns>
		ActionList& operator=(const ActionList&) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="">ActionList to move</param>
		/// <returns>Reference to ActionList that was moved</returns>
		ActionList& operator=(ActionList&&) noexcept = default;

		/// <summary>
		/// Defaulted Destructor
		/// </summary>
		virtual ~ActionList() = default;

		/// <summary>
		/// Invoke update on all actions in action list
		/// </summary>
		/// <param name="gameTime"></param>
		void Update(const GameTime& gameTime) override;

		/// <summary>
		/// Create an action and adopt it into prescribed actions datum
		/// </summary>
		/// <param name="className"> - name of class to create</param>
		/// <param name="name"> - name of action</param>
		Action& CreateAction(const std::string& className, const std::string& name);

		/// <summary>
		/// Signatures of action list
		/// </summary>
		/// <returns>Vector of signatures</returns>
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	protected:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="typeId"> type id</param>
		ActionList(RTTI::IdType typeId);
	};

	CONCRETE_FACTORY(ActionList, FieaGameEngine::Scope);
}

