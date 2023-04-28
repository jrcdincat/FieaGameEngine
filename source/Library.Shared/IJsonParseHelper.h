#pragma once
#include "RTTI.h"
#include <string>
#include <memory>
#include "json/json.h"
#include "JsonParseCoordinator.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Abstract base class for json parse helpers
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		using size_type = uint32_t;
	public:
		/// <summary>
		/// Context class maintains context while parsing json
		/// </summary>
		class Context
		{
		public:
			/// <summary>
			/// Constructor
			/// </summary>
			/// <param name="key"> - key name within this context</param>
			/// <param name="scope"> - either root, or previously created scope</param>
			Context(const std::string& key, Scope* scope, Datum* datum = nullptr);

			/// <summary>
			/// Key name of this attribute
			/// </summary>
			const std::string* _key;

			/// <summary>
			/// Datum that was appended onto scope
			/// </summary>
			Datum* _datum{ nullptr };

			/// <summary>
			/// Scope that is either root, or the previously created scope
			/// </summary>
			Scope* _scope;

			/// <summary>
			/// Class name
			/// </summary>
			std::string _className;
		};

		/// <summary>
		/// Initializes this helper
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Cleanup this helper
		/// </summary>
		virtual void Cleanup();

		/// <summary>
		/// Attempts to handle the json key value pair
		/// </summary>
		/// <param name="wrapper"> - wrapper for data</param>
		/// <param name="key"> - name associated with data </param>
		/// <param name="value"> - json value to be stored as data</param>
		/// <param name="isArrayElement"> - whether or not it is an array</param>
		/// <returns>True or False</returns>
		virtual [[nodiscard]] bool StartHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value) = 0;

		/// <summary>
		/// Attempts to complete the handling of the key value pair element
		/// </summary>
		/// <param name="wrapper"> - wrapper for data</param>
		/// <param name="key"> - name associated with the data</param>
		/// <returns>True or False</returns>
		virtual [[nodiscard]] bool EndHandler(const std::string& key) = 0;

		/// <summary>
		/// Creates and instance of the helper
		/// </summary>
		/// <returns>Shared ptr of IJsonParseHelper</returns>
		virtual std::shared_ptr<IJsonParseHelper> Create() const = 0;
	};
}