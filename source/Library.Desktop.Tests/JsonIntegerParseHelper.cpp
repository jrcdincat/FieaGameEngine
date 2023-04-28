#include "pch.h"
#include "JsonIntegerParseHelper.h"
#include "JsonParseCoordinator.h"

namespace UnitTests
{
    RTTI_DEFINITIONS(JsonIntegerParseHelper);

    std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> JsonIntegerParseHelper::Wrapper::Create() const
    {
        return std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper>();
    }

    std::shared_ptr<FieaGameEngine::IJsonParseHelper> JsonIntegerParseHelper::Create() const
    {
        return std::make_shared<JsonIntegerParseHelper>();
    }

    bool JsonIntegerParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value)
    {
        _startHandlerCount++;
        if (!wrapper.As<JsonIntegerParseHelper::Wrapper>())
        {
            return false;
        }
        
        if (wrapper.Data.Type() != FieaGameEngine::Datum::DatumTypes::Integer)
        {
            wrapper.Data.SetType(FieaGameEngine::Datum::DatumTypes::Integer);
        }
       
        wrapper.Data = value.asInt();
        
        key;
        return true;
    }

    bool JsonIntegerParseHelper::EndHandler(const std::string& key)
    {
        _endHandlerCount++;
        key;
        return false;
    }
}
