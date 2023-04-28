#pragma once
#include "ActionList.h"
#include "IEventSubscriber.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Reaction inherits from ActionList and IEventSubscriber
	/// </summary>
	class Reaction : public ActionList, public IEventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);

	public:
		/// <summary>
		/// Update
		/// </summary>
		/// <param name="gameTime"></param>
		void Update(const GameTime& gameTime) override = 0;

		/// <summary>
		/// Defaulted destructor
		/// </summary>
		virtual ~Reaction() override = default;

		/// <summary>
		/// Signatures of ReactionAttributed
		/// </summary>
		/// <returns>Vector of signatures</returns>
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	protected: 
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="typeId"> type id</param>
		Reaction(RTTI::IdType typeId);
	};
}