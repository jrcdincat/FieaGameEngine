#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	using DatumTypes = Datum::DatumTypes;

	RTTI_DEFINITIONS(ActionList);

	ActionList::ActionList() : Action(TypeIdClass())
	{
	}

	void ActionList::Update(const GameTime& gameTime)
	{
		Datum& actions = (*this)["Actions"];
		for (size_type index = 0; index < actions.Size(); ++index)
		{
			Scope& actionScope = actions.GetScope();
			assert(actionScope.Is(Action::TypeIdClass()));
			actionScope;
			Action& action = static_cast<Action&>(actionScope);
			action.Update(gameTime);
		}
	}

	Action& ActionList::CreateAction(const std::string& className, const std::string& name)
	{
		Scope* actionScope = Factory<Scope>::Create(className);
		assert(actionScope != nullptr);
		assert(actionScope->Is(Action::TypeIdClass()));
		Action& action = static_cast<Action&>(*actionScope);
		action.SetName(name);
		Adopt(*actionScope, "Actions");
		return action;
	}

	Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			{ "Actions", DatumTypes::Table, 0, 0 },
		};
	}

	ActionList::ActionList(RTTI::IdType typeId) : Action(typeId)
	{
	}
}