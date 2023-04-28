#include "pch.h"
#include "JsonTestParseHelper.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(JsonTestParseHelper);
	RTTI_DEFINITIONS(UnitTests::JsonTestParseHelper::Wrapper);

	std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> JsonTestParseHelper::Wrapper::Create() const
	{ 
		return std::make_shared<JsonTestParseHelper::Wrapper>();
	}

	std::shared_ptr<FieaGameEngine::IJsonParseHelper> JsonTestParseHelper::Create() const
	{
		return std::make_shared<JsonTestParseHelper>();
	}

	bool JsonTestParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value)
	{
		value;
		key;
		wrapper;
		_startHandlerCount++;
		return true;
	}

	bool JsonTestParseHelper::EndHandler(const std::string& key)
	{
		_endHandlerCount++;
		key;
		return true;
	}
}
