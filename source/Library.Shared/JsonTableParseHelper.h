#pragma once
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include <memory>
#include <stack>
#include "Vector.h"
#include "Scope.h"
#include <stack>
#include <utility>

namespace FieaGameEngine
{
	/// <summary>
	/// Helper class that assists with converting json values into scope/datum values
	/// </summary>
	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper);
		using size_type = uint32_t;

	public:
		/// <summary>
		/// Wrapper that holds root of the data converted from Json
		/// </summary>
		class Wrapper final : public JsonParseCoordinator::Wrapper
		{
			using size_type = uint32_t;
			RTTI_DECLARATIONS(Wrapper, JsonParseCoordinator::Wrapper);

		public:
			Wrapper(std::shared_ptr<Scope> root); 

			/// <summary>
			/// Create new instance of this wrapper
			/// </summary>
			/// <returns>New shared pointer of JsonTableParseHelper Wrapper</returns>
			std::shared_ptr<JsonParseCoordinator::Wrapper> Create() const override;

			/// <summary>
			/// Root scope
			/// </summary>
			std::shared_ptr<Scope> _root;
		};

		/// <summary>
		/// Starts handling Json data. Creates and sets the data values from json.
		/// </summary>
		/// <param name="wrapper"> - wrapper that contains the root scope</param>
		/// <param name="key"> - key name, could be keywords or attribute name</param>
		/// <param name="value"> - json value associated with key</param>
		/// <returns></returns>
		bool StartHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value) override;

		/// <summary>
		/// End handling pops context frames off context stack
		/// </summary>
		/// <param name="key"> - key name</param>
		/// <returns>True</returns>
		bool EndHandler(const std::string& key) override;

		/// <summary>
		/// Create new instance of this helper
		/// </summary>
		/// <returns>New shared pointer of JsonTableParseHelper</returns>
		std::shared_ptr<IJsonParseHelper> Create() const override;

	private:
		/// <summary>
		/// Helper function to set datum type from json value type 
		/// </summary>
		/// <param name="context"> - context frame</param>
		/// <param name="value"> - value to get type from</param>
		void SetType(Context& context, const Json::Value& value);

		/// <summary>
		/// Helper function to set datum value from converted json value
		/// </summary>
		/// <param name="context"> - context frame</param>
		/// <param name="value"> - value to set datum equal to</param>
		void SetValue(Context& context, const Json::Value& value);
		
		/// <summary>
		/// Context stack
		/// </summary>
		std::stack<Context, Vector<Context>>_contextStack;
	};
}

