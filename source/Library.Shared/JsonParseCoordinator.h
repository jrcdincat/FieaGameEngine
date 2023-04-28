#pragma once
#include <string>
#include "Datum.h"
#include "RTTI.h"
#include "json/json.h"
#include <memory>

namespace FieaGameEngine
{
	class IJsonParseHelper;

	/// <summary>
	/// Handles the parsing of json
	/// </summary>
	class JsonParseCoordinator final
	{
	public:
		/// <summary>
		/// Wrapper represents the data that helpers share with each other and with coordinator
		/// </summary>
		class Wrapper : public RTTI
		{
			friend JsonParseCoordinator;

			RTTI_DECLARATIONS(Wrapper, RTTI);
		public:
			using size_type = uint32_t;

			/// <summary>
			/// Initialize the wrapper
			/// </summary>
			virtual void Initialize();

			/// <summary>
			/// Cleanup the wrapper
			/// </summary>
			virtual void Cleanup();

			/// <summary>
			/// Create an instance of the wrapper
			/// </summary>
			/// <returns>Shared ptr wrapper</returns>
			virtual std::shared_ptr<Wrapper> Create() const = 0;

			/// <summary>
			/// Returns the current depth
			/// </summary>
			/// <returns>Depth</returns>
			virtual size_type Depth();

			/// <summary>
			/// Returns the max depth
			/// </summary>
			/// <returns>Max Depth</returns>
			virtual size_type MaxDepth();


			/// <summary>
			/// Location where data is currently stored. TODO: Needs to be updated for scopes
			/// </summary>
			Datum Data; // TODO: Needs to be removed, but requires updating IntegerParseHelper tests

		private:
			/// <summary>
			/// Increment the current depth (Descend)
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// Decrement the current depth (Ascend)
			/// </summary>
			void DecrementDepth();

			/// <summary>
			/// Depth while parsing through json
			/// </summary>
			size_type _depth;

			/// <summary>
			/// Maximum depth while parsing through the json
			/// </summary>
			size_type _maxDepth;
		};

		/// <summary>
		/// Deleted default constructor to force user to use constructor that takes in wrapper
		/// </summary>
		JsonParseCoordinator() = delete;

		/// <summary>
		/// Constructor that takes in a wrapper
		/// </summary>
		/// <param name="wrapper"> - wrapper to set inside parse coordinator</param>
		explicit JsonParseCoordinator(std::shared_ptr<Wrapper> wrapper);

		/// <summary>
		/// Deleted copy constructor
		/// </summary>
		/// <param name="other"> - parse coordinator to copy</param>
		JsonParseCoordinator(const JsonParseCoordinator& other) = delete;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="other"> - parse coordinator to move</param>
		JsonParseCoordinator(JsonParseCoordinator&& other) noexcept = default;

		/// <summary>
		/// Deleted copy assignment operator
		/// </summary>
		/// <param name="rhs"> - parse coordinator to copy</param>
		/// <returns>Reference to copy of the parse coordinator</returns>
		JsonParseCoordinator& operator=(const JsonParseCoordinator& rhs) = delete;

		/// <summary>
		/// Default move assignment operator
		/// </summary>
		/// <param name="rhs"> - parse coordinator to move</param>
		/// <returns>Reference of the parse coordinator after move</returns>
		JsonParseCoordinator& operator=(JsonParseCoordinator&& rhs) noexcept = default;

		/// <summary>
		/// Default destructor
		/// </summary>
		~JsonParseCoordinator() = default;

		/// <summary>
		/// Initialize wrapper and helpers
		/// </summary>
		void Initialize();

		/// <summary>
		/// Cleanup the helpers
		/// </summary>
		void Cleanup();

		/// <summary>
		/// Create a new duplicated version of the parse coordinator, not a deep copy
		/// </summary>
		/// <returns>shared_ptr of parse coordinator clone</returns>
		std::unique_ptr<JsonParseCoordinator> Clone();

		/// <summary>
		/// Add a helper to the parse coordinator
		/// </summary>
		/// <param name="jsonParseHelper"> - helper to add</param>
		void AddHelper(std::shared_ptr<IJsonParseHelper> jsonParseHelper);

		/// <summary>
		/// Remove a given helper from the parse coordinator
		/// </summary>
		/// <param name="jsonParseHelper"> - helper to remove</param>
		void RemoveHelper(RTTI::IdType typeId);

		/// <summary>
		/// Deserializes an object from a string of json data
		/// </summary>
		/// <param name="jsonData"></param>
		void DeserializeObject(const std::string& jsonData);

		/// <summary>
		/// Starts parsing from values stored in stream
		/// </summary>
		/// <param name="stream"></param>
		void DeserializeObject(std::istream& stream);

		/// <summary>
		/// Deserializes an object from file path
		/// </summary>
		/// <param name="filePath"> - location or name of file</param>
		void DeserializeObjectFromFile(const std::string& filePath);

		/// <summary>
		/// Returns helpers for this parse coordinator
		/// </summary>
		/// <returns>Vector of shared ptrs of IJsonParseHelper</returns>
		Vector <std::shared_ptr<IJsonParseHelper>> Helpers();

		/// <summary>
		/// Get wrapper associated with this parse coordinator
		/// </summary>
		/// <returns>Parse coordinator wrapper</returns>
		[[nodiscard]] Wrapper& GetWrapper();

		/// <summary>
		/// Set wrapper for this parse coordinator
		/// </summary>
		/// <param name="wrapper"></param>
		void SetWrapper(std::shared_ptr<Wrapper> wrapper);


	private:
		/// <summary>
		/// Parse through all the members from the JsonValue. This means traversing through all the json.
		/// </summary>
		/// <param name="jsonValue"> - value to parse</param>
		//void ParseMembers(const Json::Value& jsonValue);

		/// <summary>
		/// Parse through and start and end handlers for single attribute
		/// </summary>
		/// <param name="key"> - name</param>
		/// <param name="jsonValue"> - jsonValue to store</param>
		/// <param name="isElementOfArray"> - whether it is an array</param>
		void Parse(const Json::Value& jsonValue);

		void ParseValue(const std::string& key, const Json::Value& value);

		/// <summary>
		/// Wrapper associated with this parse coordinator
		/// </summary>
		std::shared_ptr<Wrapper> _wrapper;

		/// <summary>
		/// Parse helpers that handle specific data types 
		/// </summary>
		Vector <std::shared_ptr<IJsonParseHelper>> _helpers;
	};
}