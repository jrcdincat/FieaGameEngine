#include "pch.h"
#include "Monster.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Monster);
	Monster::Monster() : GameObject(TypeIdClass())
	{
	}
	void Monster::Update(const GameTime& gameTime)
	{
		GameObject::Update(gameTime);
		_updateCount++;
	}
}