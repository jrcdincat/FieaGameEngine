#include "pch.h"
#include "IJsonParseHelper.h"


namespace FieaGameEngine
{
	IJsonParseHelper::Context::Context(const std::string& key, Scope* scope, Datum* datum) :
		_key{ &key }, _scope { scope }, _datum(datum)
	{
	}

	void IJsonParseHelper::Initialize()
	{
	}

	void IJsonParseHelper::Cleanup()
	{
	}
}