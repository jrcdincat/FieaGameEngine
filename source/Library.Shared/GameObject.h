#pragma once
#include "Attributed.h"
#include <glm/glm.hpp>
#include "GameTime.h"
#include <gsl/gsl>
#include "ConcreteFactory.h"
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Base game object class
	/// </summary>
	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed);

	public: 
		/// <summary>
		/// Default constrctor
		/// </summary>
		GameObject(); 

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="">Game object to copy</param>
		GameObject(const GameObject&) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="">Game object to move</param>
		GameObject(GameObject&&) = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="">Game object to copy</param>
		/// <returns>Copied  game object reference</returns>
		GameObject& operator=(const GameObject&) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="">Game object to move</param>
		/// <returns>Moved game object</returns>
		GameObject& operator=(GameObject&&) = default;

		/// <summary>
		/// Defaulted destructor
		/// </summary>
		virtual ~GameObject() = default;

		/// <summary>
		/// Game object update that calls update on all children
		/// </summary>
		/// <param name="">Game time</param>
		virtual void Update(const GameTime&); 

		/// <summary>
		/// Name of game object
		/// </summary>
		std::string Name = ""s;

		/// <summary>
		/// Transform of game object (position, rotation, scale)
		/// </summary>
		glm::mat4 Transform = glm::mat4(0);

		/// <summary>
		/// Clone game object
		/// </summary>
		/// <returns>Cloned scope*</returns>
		gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Check if RTTI* is equivalent to this object
		/// </summary>
		/// <param name="rhs"> - pointer to compare</param>
		/// <returns>True or False</returns>
		bool Equals(const RTTI* rhs) const;

		/// <summary>
		/// Create an action and adopt it into prescribed actions datum
		/// </summary>
		/// <param name="className"> - name of class to create</param>
		/// <param name="name"> - name of action</param>
		Action& CreateAction(const std::string& className, const std::string& name);

		/// <summary>
		/// Create an child and adopt it into prescribed actions datum
		/// </summary>
		/// <param name="className"> - name of class to create</param>
		/// <param name="name"> - name of child</param>
		GameObject& CreateChild(const std::string& className, const std::string& name);

		/// <summary>
		/// Gets reference to prescribed Actions datum
		/// </summary>
		/// <returns>Actions datum reference</returns>
		Datum& Actions();

		/// <summary>
		/// Signatures of game object
		/// </summary>
		/// <returns>Vectore of signatures</returns>
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		/// <summary>
		/// For testing update calls.
		/// </summary>
		int32_t _updateCount = 0;
	protected: 

		/// <summary>
		/// Constructor of game object
		/// </summary>
		/// <param name="typeId"> type id of derived type passed into constructor</param>
		GameObject(RTTI::IdType typeId) : Attributed(typeId) { };
	};

	CONCRETE_FACTORY(GameObject, FieaGameEngine::Scope);
}