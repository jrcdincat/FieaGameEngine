#pragma once
#include "Datum.h"
#include "IJsonParseHelper.h"
#include "JsonParseCoordinator.h"
#include <memory>

namespace UnitTests
{
	class JsonIntegerParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		using size_type = uint32_t;
		RTTI_DECLARATIONS(JsonIntegerParseHelper, IJsonParseHelper);

	public:
		class Wrapper : public FieaGameEngine::JsonParseCoordinator::Wrapper
		{
			using size_type = int32_t;

		public:
			//virtual void Initialize() override { Data.SetType(FieaGameEngine::Datum::DatumTypes::Integer); };

			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> Create() const override;
		};

		std::shared_ptr<IJsonParseHelper> Create() const;

		[[nodiscard]] bool StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value) override;
		[[nodiscard]] bool EndHandler(const std::string& key) override;

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
