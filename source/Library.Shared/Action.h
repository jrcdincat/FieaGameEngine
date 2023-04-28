#pragma once
#include "RTTI.h"
#include "Attributed.h"
#include "Factory.h"
#include "GameTime.h"

namespace FieaGameEngine
{		
	class GameObject;
	/// <summary>
	/// Action abstract base class
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);

	public: 

		/// <summary>
		/// Deleted default constructor
		/// </summary>
		Action() = delete;

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="">Action to copy</param>
		Action(const Action&) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="">Action to move</param>
		Action(Action&&) noexcept = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="">Action to copy</param>
		/// <returns>Reference to action copy</returns>
		Action& operator=(const Action&) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="">Action to move</param>
		/// <returns>Reference to moved action</returns>
		Action& operator=(Action&&) noexcept = default;

		/// <summary>
		/// Pure virtual update method for actions
		/// </summary>
		/// <param name="gametime"> - Game time</param>
		virtual void Update(const GameTime& gametime) = 0;

		/// <summary>
		/// Signatures of Action
		/// </summary>
		/// <returns>Vector of signatures</returns>
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		/// <summary>
		/// Gets name of action
		/// </summary>
		/// <returns>Name</returns>
		const std::string Name() const;

		/// <summary>
		/// Sets name of action
		/// </summary>
		/// <param name="name"> - Name to set actions name to</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Virtual defaulted destructor
		/// </summary>
		virtual ~Action() = default;

	protected: 
		/// <summary>
		/// Constructor of Action
		/// </summary>
		/// <param name="typeId">type id of derived type passed into constructor</param>
		Action(RTTI::IdType typeId) : Attributed(typeId) { };
	
	private: 
		/// <summary>
		/// Name of action
		/// </summary>
		std::string _name = "";
	};
}