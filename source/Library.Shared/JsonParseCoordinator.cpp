#include "pch.h"
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include <sstream>
#include <fstream>
#include <istream>
#include "Scope.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseCoordinator::Wrapper);

	void JsonParseCoordinator::Wrapper::Initialize()
	{
		_depth = 1;
		_maxDepth = 1;
	}

	void JsonParseCoordinator::Wrapper::Cleanup()
	{
	}

	JsonParseCoordinator::Wrapper::size_type JsonParseCoordinator::Wrapper::Depth()
	{
		return _depth;
	}

	JsonParseCoordinator::Wrapper::size_type JsonParseCoordinator::Wrapper::MaxDepth()
	{
		return _maxDepth;
	}

	void JsonParseCoordinator::Wrapper::IncrementDepth()
	{
		_depth++;
		if (_depth > _maxDepth)
		{
			_maxDepth = _depth;
		}
	}

	void JsonParseCoordinator::Wrapper::DecrementDepth()
	{
		_depth--;
	}

	JsonParseCoordinator::JsonParseCoordinator(std::shared_ptr<Wrapper> wrapper) : _wrapper{ std::move(wrapper) }
	{
	}

	std::unique_ptr<JsonParseCoordinator> JsonParseCoordinator::Clone()
	{
		std::unique_ptr<JsonParseCoordinator> cloneCoordinator = std::make_unique<JsonParseCoordinator>(_wrapper->Create());

		cloneCoordinator->_helpers.Reserve(_helpers.size());
		for (auto& helper : _helpers)
		{
			cloneCoordinator->_helpers.push_back(helper->Create());
		}
		return cloneCoordinator;
	}

	void JsonParseCoordinator::AddHelper(std::shared_ptr<IJsonParseHelper> jsonParseHelper)
	{
		auto position = std::find_if(_helpers.begin(), _helpers.end(), [&jsonParseHelper](const std::shared_ptr<IJsonParseHelper>& h)
			{
				return jsonParseHelper->TypeIdInstance() == h->TypeIdInstance();
			});

		if (position != _helpers.end())
		{
			throw std::runtime_error("Helper with given type has already been added to this JsonParseCoordinator.");
		}

		_helpers.push_back(std::move(jsonParseHelper));
	}

	void JsonParseCoordinator::RemoveHelper(RTTI::IdType typeId)
	{
		auto position = std::find_if(_helpers.begin(), _helpers.end(), [typeId](const std::shared_ptr<IJsonParseHelper>& h)
			{
				return typeId == h->TypeIdInstance();
			});

		_helpers.Remove(position);
	}

	void JsonParseCoordinator::DeserializeObject(const std::string& jsonData)
	{
		std::istringstream stringStream{ jsonData };
		DeserializeObject(stringStream);
	}

	void JsonParseCoordinator::DeserializeObject(std::istream& inputStream)
	{
		Initialize();
		Json::Value root;
		assert(inputStream.good());
		inputStream >> root;
		Parse(root);
		Cleanup();
	}

	void JsonParseCoordinator::DeserializeObjectFromFile(const std::string& filePath)
	{
		std::ifstream fileStream{ filePath };
		DeserializeObject(fileStream);
	}

	Vector<std::shared_ptr<IJsonParseHelper>> JsonParseCoordinator::Helpers()
	{
		return _helpers;
	}

	JsonParseCoordinator::Wrapper& JsonParseCoordinator::GetWrapper()
	{
		return *_wrapper;
	}

	void JsonParseCoordinator::SetWrapper(std::shared_ptr<Wrapper> wrapper)
	{
		_wrapper = std::move(wrapper);
	}

	void JsonParseCoordinator::Initialize()
	{
		_wrapper->Initialize();

		for (auto& helper : _helpers)
		{
			helper->Initialize();
		}
	}

	void JsonParseCoordinator::Cleanup()
	{
		for (auto& helper : _helpers)
		{
			helper->Cleanup();
		}

		_wrapper->Cleanup();
	}

	void JsonParseCoordinator::Parse(const Json::Value& jsonValue)
	{
		auto members = jsonValue.getMemberNames();
		for (size_t index = 0; index < members.size(); ++index)
		{
			const auto& key = members[index];
			ParseValue(key, jsonValue[key]);
		}
	}

	void JsonParseCoordinator::ParseValue(const std::string& key, const Json::Value& value)
	{
		if (value.isObject())
		{
			_wrapper->IncrementDepth();

			for (auto& helper : _helpers)
			{
				if (helper->StartHandler(*_wrapper, key, value))
				{
					Parse(value);
					helper->EndHandler(key);
					break;
				}
			}

			_wrapper->DecrementDepth();
		}
		else if (value.isArray())
		{
			for (const auto& element : value)
			{
				if (element.isObject())
				{
					_wrapper->IncrementDepth();
					Parse(element);
					_wrapper->DecrementDepth();
				}
				else
				{
					ParseValue(key, element);
				}
			}
		}
		else
		{
			for (auto& helper : _helpers)
			{
				if (helper->StartHandler(*_wrapper, key, value))
				{
					helper->EndHandler(key);
					break;
				}
			}
		}
	}
}