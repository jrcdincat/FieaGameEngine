#include "pch.h"
#include "JsonTableParseHelper.h"
#include <string>
#include "Factory.h"

using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonTableParseHelper);
	RTTI_DEFINITIONS(JsonTableParseHelper::Wrapper);

	JsonTableParseHelper::Wrapper::Wrapper(std::shared_ptr<Scope> root) : _root{ root }
	{
	}

	std::shared_ptr<JsonParseCoordinator::Wrapper> JsonTableParseHelper::Wrapper::Create() const
	{
		return std::make_shared<JsonTableParseHelper::Wrapper>(std::make_shared<Scope>()); // Is this valid?
	}

	std::shared_ptr<IJsonParseHelper> JsonTableParseHelper::Create() const
	{
		return std::make_shared<JsonTableParseHelper>();
	}

	bool JsonTableParseHelper::StartHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value)
	{
		auto tableWrapper = wrapper.As<JsonTableParseHelper::Wrapper>();
		if (tableWrapper == nullptr)
		{
			return false;
		}

		if (key == "type")
		{
			assert(_contextStack.size() > 0);
			SetType(_contextStack.top(), value);
		}
		else if (key == "value")
		{
			assert(_contextStack.size() > 0);
			SetValue(_contextStack.top(), value);
		}
		else if (key == "class")
		{
			const auto& classname = value.asString();
			_contextStack.top()._className = classname;

		}
		else
		{
			Scope* scope = _contextStack.size() == 0 ? tableWrapper->_root.get() : _contextStack.top()._scope;
		    Datum& datum = scope->Append(key);
			_contextStack.push({ key, scope, &datum });
		}

		return true;
	}

	bool JsonTableParseHelper::EndHandler(const std::string& key)
	{
		auto* kes = &key;
		kes;
		if (_contextStack.size() > 0)
		{
			if (_contextStack.top()._key == &key)
			{
				_contextStack.pop();
			}
		}

		return true;
	}

	void JsonTableParseHelper::SetType(Context& context, const Json::Value& value)
	{
		assert(context._datum != nullptr);
		std::string type = value.asString();
		context._datum->SetType(Datum::StringDatumTypesMap.At(type));
	}

	void JsonTableParseHelper::SetValue(Context& context, const Json::Value& value)
	{
		assert(context._datum != nullptr);

		if (context._datum != nullptr)
		{
			if (context._datum->IsExternal())
			{
				switch (context._datum->Type())
				{
				case Datum::DatumTypes::Integer:
					context._datum->Set(value.asInt());
					break;
				case Datum::DatumTypes::Float:
					context._datum->Set(value.asFloat());
					break;
				case Datum::DatumTypes::String:
					context._datum->Set(value.asString());
					break;
				case Datum::DatumTypes::Vector:
				{
					const std::string& vectorString = value.asString();
					context._datum->SetFromString(vectorString);
					break;
				}
				case Datum::DatumTypes::Matrix:
				{
					const std::string& matrixString = value.asString();
					context._datum->SetFromString(matrixString);
					break;
				}
				default:
					break;
				}
			}
			else
			{
				switch (context._datum->Type())
				{
				case Datum::DatumTypes::Integer:
					context._datum->PushBack(value.asInt());
					break;
				case Datum::DatumTypes::Float:
					context._datum->PushBack(value.asFloat());
					break;
				case Datum::DatumTypes::String:
					context._datum->PushBack(value.asString());
					break;
				case Datum::DatumTypes::Vector:
				{
					context._datum->PushBack(glm::vec4(0));
					const std::string& vectorString = value.asString();
					context._datum->SetFromString(vectorString);
					break;
				}
				case Datum::DatumTypes::Matrix:
				{
					context._datum->PushBack(glm::mat4(0));
					const std::string& matrixString = value.asString();
					context._datum->SetFromString(matrixString);
					break;
				}
				case Datum::DatumTypes::Table: 
				{
					Scope* scope;
					if (context._className.empty())
					{
						scope = &context._scope->AppendScope(*context._key);
					}
					else
					{
						scope = Factory<Scope>::Create(context._className);
						assert(scope != nullptr);
						context._scope->Adopt(*scope, *context._key);
						
					}
					/*Context newContext{ *context._key, scope, context._datum };
					_contextStack.push(newContext);*/
				}
				default:
					break;
				}
			}
		}
	}
}
