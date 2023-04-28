#pragma once
#include "GameObject.h"

namespace FieaGameEngine
{
	class Monster final : public GameObject
	{
		RTTI_DECLARATIONS(Monster, GameObject);
	public:
		Monster();
		Monster(const Monster&) = default;
		Monster(Monster&&) = default;
		Monster& operator=(const Monster&) = default;
		Monster& operator=(Monster&&) = default;

		void Update(const GameTime&) override;
	};

	CONCRETE_FACTORY(Monster, FieaGameEngine::Scope);
}