#include "pch.h"
#include "GameObject.h"
#include "Datum.h"
#include <iostream>

namespace FieaGameEngine
{
	using DatumTypes = Datum::DatumTypes;

	RTTI_DEFINITIONS(GameObject);

	GameObject::GameObject() : Attributed(TypeIdClass()), Name{""}, Transform{ glm::mat4(0) }
	{
	}

	gsl::owner<Scope*> GameObject::Clone() const
	{
		return gsl::owner<GameObject*>(new GameObject(*this));
	}

	bool GameObject::Equals(const RTTI* rhs) const
	{
		const auto rhsFoo = rhs->As<GameObject>();
		if (rhsFoo == nullptr)
		{
			return false;
		}

		return Name == rhsFoo->Name &&
			Transform == rhsFoo->Transform;
	}

	void GameObject::Update(const GameTime& gameTime)
	{
		Datum& children = (*this)["Children"];
		
		for (size_type index = 0; index < children.Size(); ++index)
		{
			Scope& child = children.GetScope();
			assert(child.Is(GameObject::TypeIdClass()));
			GameObject* object = static_cast<GameObject*>(&child);
			object->Update(gameTime);
		}

		Datum& actions = (*this)["Actions"];
		for (size_type index = 0; index < actions.Size(); ++index)
		{
			Scope& actionScope = actions.GetScope();
			assert(actionScope.Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(&actionScope);
			action->Update(gameTime);
		}
	}

	Action& GameObject::CreateAction(const std::string& className, const std::string& name)
	{
		Scope* actionScope = Factory<Scope>::Create(className);
		assert(actionScope != nullptr);
		assert(actionScope->Is(Action::TypeIdClass()));
		Action& action = static_cast<Action&>(*actionScope);
		action.SetName(name);
		Adopt(action, "Actions");
		return action;
	}

	GameObject& GameObject::CreateChild(const std::string& className, const std::string& name)
	{
		Scope* childScope = Factory<Scope>::Create(className);
		assert(childScope != nullptr);
		assert(childScope->Is(GameObject::TypeIdClass()));
		GameObject& child = static_cast<GameObject&>(*childScope);
		child.Name = name;
		Adopt(child, "name");
		return child;
	}

	Datum& GameObject::Actions()
	{
		return (*this)["Actions"];
	}


	Vector<Signature> GameObject::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", DatumTypes::String, 1, offsetof(GameObject, Name) }, 
			{ "Transform", DatumTypes::Matrix, 1, offsetof(GameObject, Transform) }, 
			{ "Children", DatumTypes::Table, 0, 0},
			{ "Actions", DatumTypes::Table, 0, 0 }
		};
	}
}
