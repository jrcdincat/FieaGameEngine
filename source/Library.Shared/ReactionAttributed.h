#pragma once
#include "Reaction.h"
#include "ConcreteFactory.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Reaction Attributed
	/// </summary>
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);
	public:
		/// <summary>
		/// Reaction Attributed
		/// </summary>
		ReactionAttributed();

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">- to copy</param>
		ReactionAttributed(const ReactionAttributed& other) = default;

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="other">- to move</param>
		ReactionAttributed(ReactionAttributed&& other) = default;
		
		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="rhs">ReactionAttributed to copy</param>
		/// <returns> - Reference to ReactionAttributed copy</returns>
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="rhs">ReactionAttributed to move</param>
		/// <returns> - Reference to moved ReactionAttributed</returns>
		ReactionAttributed& operator=(ReactionAttributed&& rhs) noexcept = default;

		/// <summary>
		/// Notify
		/// </summary>
		/// <param name="attributedEvent"> event</param>
		void Notify(EventPublisher& attributedEvent) override;

		/// <summary>
		/// Destructor
		/// </summary>
		~ReactionAttributed() override = default;

		/// <summary>
		/// Update
		/// </summary>
		/// <param name="gameTime"></param>
		void Update(const GameTime& gameTime) override;

		/// <summary>
		/// Set SubType
		/// </summary>
		void SetSubType(const std::string& subType);

		/// <summary>
		/// Signatures of ReactionAttributed
		/// </summary>
		/// <returns>Vector of signatures</returns>
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	private: 
		/// <summary>
		/// SubType
		/// </summary>
		std::string SubType;

	};
	
	CONCRETE_FACTORY(ReactionAttributed, Scope);
}