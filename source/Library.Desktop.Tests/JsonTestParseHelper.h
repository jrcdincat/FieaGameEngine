#pragma once
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include <memory>
#include <stack>
#include "Vector.h"
#include "Datum.h"

namespace UnitTests
{
	class JsonTestParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTestParseHelper, IJsonParseHelper);
		using size_type = uint32_t;

	public:
		class Wrapper final : public FieaGameEngine::JsonParseCoordinator::Wrapper
		{
			using size_type = uint32_t;
			RTTI_DECLARATIONS(Wrapper, FieaGameEngine::JsonParseCoordinator::Wrapper);

		public:
			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> Create() const override;
		};

		bool StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value);
		bool EndHandler(const std::string& key) override;

		std::shared_ptr<IJsonParseHelper> Create() const override;

		size_type StartHandlerCount()  {
			return _startHandlerCount;
		}
		size_type EndHandlerCount()  {
			return _endHandlerCount;
		}
	private:
		size_type _startHandlerCount{ 0 };
		size_type _endHandlerCount{ 0 };
		bool _initializeCalled{ false };
	};
}

