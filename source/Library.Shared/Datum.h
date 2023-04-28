#pragma once

#include <cstddef>
#include <string>
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "RTTI.h"
#include "HashMap.h"
#include <functional>

namespace FieaGameEngine
{
	class Scope;

	/// <summary>
	/// Stores an array of values with a single given type. 
	/// Different datum objects can store values of different types. 
	/// </summary>
	class Datum final
	{
	public:
		friend class Scope;
		using size_type = std::size_t;
		enum class DatumTypes
		{
			Unknown = 0,
			Integer,
			Float,
			String,
			Table,
			Vector,
			Matrix,
			Pointer,
			Byte,

			End = Pointer
		};

		static const HashMap<Datum::DatumTypes, std::string> DatumTypesStringMap;
		static const HashMap<std::string, Datum::DatumTypes> StringDatumTypesMap;

		/// <summary>
		/// Datum default constructor
		/// </summary>
		Datum() = default; 

		/// <summary>
		/// Datum constructor for setting type
		/// </summary>
		/// <param name="type"> - Type to set constructed datum as</param>
		Datum(DatumTypes type);
		
		/// <summary>
		/// Datum constructor for integer
		/// </summary>
		/// <param name="value"> - Integer to construct datum with</param>
		Datum(int value);

		/// <summary>
		/// Datum constructor for float
		/// </summary>
		/// <param name="value"> - Float to construct datum with</param>
		Datum(float value);
		
		/// <summary>
		/// Datum constructor for string
		/// </summary>
		/// <param name="value"> - String to construct datum with</param>
		Datum(const std::string& value);

		/// <summary>
		/// Datum constructor for scope
		/// </summary>
		Datum(Scope* scope);
		
		/// <summary>
		/// Datum constructor for vector
		/// </summary>
		/// <param name="value"> - Vector to construct datum with</param>
		Datum(glm::vec4 value);
		
		/// <summary>
		/// Datum constructor for matrix
		/// </summary>
		/// <param name="value"> - Matrix to construct datum with</param>
		Datum(const glm::mat4& value);

		/// <summary>
		/// Datum constructor for pointer
		/// </summary>
		/// <param name="value"> - Pointer to construct datum with</param>
		Datum(RTTI* value);
		
		/// <summary>
		/// Datum constructor with given type and capacity
		/// </summary>
		/// <param name="type"> - type to construct datum as</param>
		/// <param name="capacity"> - capacity to reserve on construction</param>
		Datum(DatumTypes type, size_type capacity);

		/// <summary>
		/// Datum copy constructor
		/// </summary>
		/// <param name="other"> - Datum to copy</param>
		Datum(const Datum& other);

		/// <summary>
		/// Datum move constructor
		/// </summary>
		/// <param name="other"> - Datum to move</param>
		Datum(Datum&& other) noexcept;

		/// <summary>
		/// Datum copy assignment operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		Datum& operator=(const Datum& rhs);

		/// <summary>
		/// Datum move assignment operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		Datum& operator=(Datum&& rhs) noexcept;
		
		/// <summary>
		/// Destructor, does nothing if datum is external storage
		/// </summary>
		~Datum(); 

		/// <summary>
		/// Integer initializer list
		/// </summary>
		/// <param name="list"> - initializer list</param>
		Datum(std::initializer_list<int> list);
		
		/// <summary>
		/// Float initializer list
		/// </summary>
		/// <param name="list"> - initializer list</param>
		Datum(std::initializer_list<float> list);
		
		/// <summary>
		/// String initializer list
		/// </summary>
		/// <param name="list"> - initializer list</param>
		Datum(std::initializer_list<std::string> list);
		
		/// <summary>
		/// Vector initializer list
		/// </summary>
		/// <param name="list"> - initializer list</param>
		Datum(std::initializer_list<glm::vec4> list);
		
		/// <summary>
		/// Matrix initializer list
		/// </summary>
		/// <param name="list"> - initializer list</param>
		Datum(std::initializer_list<glm::mat4> list);

		/// <summary>
		/// Pointer initializer list
		/// </summary>
		/// <param name="list"> - initializer list</param>
		Datum(std::initializer_list<RTTI*> list);

		/// <summary>
		/// Datum assignment operator
		/// </summary>
		/// <param name="rhs"> - Datum</param>
		/// <returns>Datum reference</returns>
		Datum& operator=(Datum& rhs);

		/// <summary>
		/// Integer assignment operator
		/// </summary>
		/// <param name="rhs"> - Integer</param>
		/// <returns>Datum reference</returns>
		Datum& operator=(const int rhs);

		/// <summary>
		/// Float assignment operator
		/// </summary>
		/// <param name="rhs"> - Float</param>
		/// <returns>Datum reference</returns>
		Datum& operator=(const float rhs);
		
		/// <summary>
		/// Matrix assignment operator
		/// </summary>
		/// <param name="rhs"> - String reference</param>
		/// <returns>Datum reference</returns>
		Datum& operator=(const std::string& rhs);

		/// <summary>
		/// Scope assignment operator
		/// </summary>
		/// <param name="rhs"> - Scope reference</param>
		/// <returns>Datum reference</returns>
		//Datum& operator=(Scope* rhs); *****************************************************
		
		/// <summary>
		/// Vector assignment operator
		/// </summary>
		/// <param name="rhs"> - Vector</param>
		/// <returns>Datum reference</returns>
		Datum& operator=(const glm::vec4 rhs);

		/// <summary>
		/// Matrix assignment operator
		/// </summary>
		/// <param name="rhs"> - Matrix</param>
		/// <returns>Datum reference</returns>
		Datum& operator=(const glm::mat4& rhs);

		/// <summary>
		/// Pointer assignment operator
		/// </summary>
		/// <param name="rhs"> - RTTI pointer</param>
		/// <returns>Datum reference</returns>
		Datum& operator=(RTTI* rhs);

#pragma region Equality
		/// <summary>
		/// Check if datum value is equal to given datum
		/// </summary>
		/// <param name="rhs"> - Datum</param>
		/// <returns>True or False</returns>
		const bool operator==(const Datum& rhs) const;

		/// <summary>
		/// Check if datum is not equal to given datum
		/// </summary>
		/// <param name="rhs"> - Datum</param>
		/// <returns>True or False</returns>
		const bool operator!=(const Datum& rhs) const;

		/// <summary>
		/// Check if datum value is equal to given integer
		/// </summary>
		/// <param name="rhs"> - integer</param>
		/// <returns>True or False</returns>
		const bool operator==(const int rhs) const;
		
		/// <summary>
		/// Check if datum value is not equal to given integer
		/// </summary>
		/// <param name="rhs"> - integer</param>
		/// <returns>True or False</returns>
		const bool operator!=(const int rhs) const;
		
		/// <summary>
		/// Check if datum value is equal to given float
		/// </summary>
		/// <param name="rhs"> - String</param>
		/// <returns>True or False</returns>
		const bool operator==(const float rhs) const;
		
		/// <summary>
		/// Check if datum value is not equal to given float
		/// </summary>
		/// <param name="rhs"> - Float</param>
		/// <returns>True or False</returns>
		const bool operator!=(const float rhs) const;

		/// <summary>
		/// Check if datum value is equal to given string
		/// </summary>
		/// <param name="rhs"> - String</param>
		/// <returns>True or False</returns>
		const bool operator==(const std::string& rhs) const;
		
		/// <summary>
		/// Check if datum value is not equal to given string
		/// </summary>
		/// <param name="rhs"> - String</param>
		/// <returns>True or False</returns>
		const bool operator!=(const std::string& rhs) const;
	
		/// <summary>
		/// Check if datum value is equal to given vector
		/// </summary>
		/// <param name="rhs"> - Vector</param>
		/// <returns>True or False</returns>
		const bool operator==(const glm::vec4 rhs) const;

		/// <summary>
		/// Check if datum value is not equal to given vector
		/// </summary>
		/// <param name="rhs"> - Vector</param>
		/// <returns>True or False</returns>
		const bool operator!=(const glm::vec4 rhs) const;
		
		/// <summary>
		/// Check if datum value is equal to given matrix
		/// </summary>
		/// <param name="rhs"> - Matrix</param>
		/// <returns>True or False</returns>
		const bool operator==(const glm::mat4 rhs) const;
		
		/// <summary>
		/// Check if datum value is not equal to given matrix
		/// </summary>
		/// <param name="rhs"> - Matrix</param>
		/// <returns>True or False</returns>
		const bool operator!=(const glm::mat4 rhs) const;
		
		/// <summary>
		/// Check if datum value is equal to given pointer
		/// </summary>
		/// <param name="rhs"> - RTTI Pointer</param>
		/// <returns>True or False</returns>
		const bool operator==(const RTTI* rhs) const;

		/// <summary>
		/// Check if datum value is not equal to given pointer
		/// </summary>
		/// <param name="rhs">RTTI Pointer</param>
		/// <returns>True or False</returns>
		const bool operator!=(const RTTI* rhs) const;

#pragma endregion

		/// <summary>
		/// Returns datum type
		/// </summary>
		const DatumTypes Type() const; 

		/// <summary>
		/// Set datum type
		/// </summary>
		/// <param name="type"> - Type that datum should be set as</param>
		void SetType(DatumTypes type);

		/// <summary>
		/// Number of elements in datum
		/// </summary>
		/// <returns>Datum size</returns>
		const size_type Size() const;

		/// <summary>
		/// Capacity of datum
		/// </summary>
		/// <returns>Datum cpacity</returns>
		const size_type Capacity() const;

		/// <summary>
		/// Allocate memory based on given capacity
		/// </summary>
		/// <param name="capacity"> - capacity amount</param>
		void Reserve(const size_type capacity);

		/// <summary>
		/// Grow or Shrink datum size to match given size
		/// </summary>
		/// <param name="size"> - size to grow or shrink to</param>
		void Resize(const size_type size);
	
		// Remove all values without changing capacity
		void Clear();

		/// <summary>
		/// Shrink datum capacity to match size
		/// </summary>
		void ShrinkToFit(); 

		/// <summary>
		/// Return whether datum is external storage
		/// </summary>
		/// <returns>True or False</returns>
		const bool IsExternal() const;

#pragma region SetStorage
		/// <summary>
		/// Set datum as external storage that points to array of data
		/// </summary>
		/// <param name="pointerArray"> - Array of data for external storage to point to</param>
		/// <param name="arraySize"> - size of given array</param>
		void SetStorage(int* intArray, const size_type arraySize); 
		// all overloads call helper function
		
		/// <summary>
		/// Set datum as external storage that points to array of data
		/// </summary>
		/// <param name="pointerArray"> - Array of data for external storage to point to</param>
		/// <param name="arraySize"> - size of given array</param>
		void SetStorage(float* floatArray, const size_type arraySize);
		
		/// <summary>
		/// Set datum as external storage that points to array of data
		/// </summary>
		/// <param name="pointerArray"> - Array of data for external storage to point to</param>
		/// <param name="arraySize"> - size of given array</param>
		void SetStorage(std::string* stringArray, const size_type arraySize);
		
		/// <summary>
		/// Set datum as external storage that points to array of data
		/// </summary>
		/// <param name="pointerArray"> - Array of data for external storage to point to</param>
		/// <param name="arraySize"> - size of given array</param>
		void SetStorage(glm::vec4* vectorArray, const size_type arraySize);
		
		/// <summary>
		/// Set datum as external storage that points to array of data
		/// </summary>
		/// <param name="pointerArray"> - Array of data for external storage to point to</param>
		/// <param name="arraySize"> - size of given array</param>
		void SetStorage(glm::mat4* matrixArray, const  size_type arraySize);

		/// <summary>
		/// Set datum as external storage that points to array of data
		/// </summary>
		/// <param name="pointerArray"> - Array of data for external storage to point to</param>
		/// <param name="arraySize"> - size of given array</param>
		void SetStorage(RTTI** pointerArray, const size_type arraySize);
#pragma endregion

#pragma region Set
		/// <summary>
		/// Set value at specified index, index defaults to 0.
		/// </summary>
		/// <param name="value"> - Integer</param>
		/// <param name="index"> - Position to set value at</param>
		void Set(int value, const size_type index = 0);
		
		/// <summary>
		/// Set value at specified index, index defaults to 0.
		/// </summary>
		/// <param name="value"> - Float</param>
		/// <param name="index"> - Position to set value at</param>
		void Set(float value, const size_type index = 0);
		
		/// <summary>
		/// Set value at specified index, index defaults to 0.
		/// </summary>
		/// <param name="value"> - String reference</param>
		/// <param name="index"> - Position to set value at</param>
		void Set(const std::string& value, const size_type index = 0);
		
		/// <summary>
		/// Set value at specified index, index defaults to 0.
		/// </summary>
		/// <param name="value"> - Scope pointer</param>
		/// <param name="index"> - Position to set value at</param>
		void Set(Scope* value, const size_type index = 0); 

		/// <summary>
		/// Set value at specified index, index defaults to 0.
		/// </summary>
		/// <param name="value"> - Vector</param>
		/// <param name="index"> - Position to set value at</param>
		void Set(glm::vec4 value, const size_type index = 0);
		
		/// <summary>
		/// Set value at specified index, index defaults to 0.
		/// </summary>
		/// <param name="value"> - Matrix</param>
		/// <param name="index"> - Position to set value at</param>
		void Set(glm::mat4 value, const size_type index = 0);

		/// <summary>
		/// Set value at specified index, index defaults to 0.
		/// </summary>
		/// <param name="value"> - RTTI Pointer</param>
		/// <param name="index"> - Position to set value at</param>
		void Set(RTTI* value, const size_type index = 0);
#pragma endregion
		/// <summary>
		/// Convert given string into datum value
		/// </summary>
		/// <param name="string"> - String to deserialize into datum value</param>
		/// <param name="index"> - index to set datum value at</param>
		void SetFromString(const std::string& string, const size_type index = 0);

		/// <summary>
		/// Convert datum to string and return it
		/// </summary>
		/// <param name="index"> - index where value to convert is at</param>
		/// <returns>String</returns>
		std::string ToString(const size_type index = 0);

#pragma region Get
		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>Integer reference</returns>
		int& GetInteger(const size_type index = 0) const;

		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>Constant integer reference</returns>
		const int GetConstInteger(const size_type index = 0) const;

		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>Float reference</returns>
		float& GetFloat(const size_type index = 0) const;

		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>Constant float</returns>
		const float GetConstFloat(const size_type index = 0) const;

		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>String reference</returns>
		std::string& GetString(const size_type index = 0) const;

		Scope& GetScope(const size_type index = 0) const;

		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>Consant string reference</returns>
		const std::string& GetConstString(const size_type index = 0) const;

		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>Vector reference</returns>
		glm::vec4& GetVector(const size_type index = 0) const;
		
		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>Constant vector</returns>
		const glm::vec4 GetConstVector(const size_type index = 0) const;
		
		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>Matrix reference</returns>
		glm::mat4& GetMatrix(const size_type index = 0) const;
		
		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>Constant matrix</returns>
		const glm::mat4 GetConstMatrix(const size_type index = 0) const;

		/// <summary>
		/// Returns value at specified index
		/// </summary>
		/// <param name="index"> - index of value to return, defaults to 0</param>
		/// <returns>RTTI Pointer</returns>
		RTTI* GetPointer(size_type index = 0) const;
#pragma endregion

#pragma region PushBack
		/// <summary>
		/// Add value to end of the data
		/// </summary>
		/// <param name="value"> - Integer</param>
		void PushBack(const int value);

		/// <summary>
		/// Add value to end of the data
		/// </summary>
		/// <param name="value"> - Float</param>
		void PushBack(const float value);
		
		/// <summary>
		/// Add value to end of the data
		/// </summary>
		/// <param name="value"> - Const string reference</param>
		void PushBack(const std::string& value);
		
		/// <summary>
		/// Add value to end of the data
		/// </summary>
		/// <param name="value"> - String rvalue reference</param>
		void PushBack(std::string&& value);

		/// <summary>
		/// Add value to end of the data
		/// </summary>
		/// <param name="value"> - Vector</param>
		void PushBack(const glm::vec4 value);
		
		/// <summary>
		/// Add value to end of the data
		/// </summary>
		/// <param name="value"> - Matrix</param>
		void PushBack(const glm::mat4& value);

		/// <summary>
		/// Add value to end of the data
		/// </summary>
		/// <param name="value"> - RTTI Pointer</param>
		void PushBack(RTTI* value);
#pragma endregion

#pragma region PopBack
		/// <summary>
		/// Remove value at end of the data
		/// </summary>
		void PopBackInteger();
		
		/// <summary>
		/// Remove value at end of the data
		/// </summary>
		void PopBackFloat();
		
		/// <summary>
		/// Remove value at end of the data
		/// </summary>
		void PopBackString();
		
		/// <summary>
		/// Remove value at end of the data
		/// </summary>
		void PopBackVector();

		/// <summary>
		/// Remove value at end of the data
		/// </summary>
		void PopBackMatrix();

		/// <summary>
		/// Remove value at end of the data
		/// </summary>
		void PopBackPointer();
#pragma endregion
		
#pragma region Front
		/// <summary>
		/// Returns the value at the front of the data
		/// </summary>
		/// <returns>Integer</returns>
		int& FrontInteger() const;

		/// <summary>
		/// Returns the value at the front of the data
		/// </summary>
		/// <returns>Float</returns>
		float& FrontFloat() const;

		/// <summary>
		/// Returns the value at the front of the data
		/// </summary>
		/// <returns>String</returns>
		std::string& FrontString() const;
		
		/// <summary>
		/// Returns the value at the front of the data
		/// </summary>
		/// <returns>Vector</returns>
		glm::vec4& FrontVector() const;

		/// <summary>
		/// Returns the value at the front of the data
		/// </summary>
		/// <returns>Matrix</returns>
		glm::mat4& FrontMatrix() const;

		/// <summary>
		/// Returns the value at the front of the data
		/// </summary>
		/// <returns>RTTI Pointer</returns>
		RTTI* FrontPointer() const;		
#pragma endregion

#pragma region Back
		/// <summary>
		/// Returns the value at the end of the data
		/// </summary>
		/// <returns>Integer</returns>
		int& BackInteger() const;

		/// <summary>
		/// Returns the value at the end of the data
		/// </summary>
		/// <returns>Float</returns>
		float& BackFloat() const;

		/// <summary>
		/// Returns the value at the end of the data
		/// </summary>
		/// <returns>String</returns>
		std::string& BackString() const;

		/// <summary>
		/// Returns the value at the end of the data
		/// </summary>
		/// <returns>Vector</returns>
		glm::vec4& BackVector() const;

		/// <summary>
		/// Returns the value at the end of the data
		/// </summary>
		/// <returns>Matrix</returns>
		glm::mat4& BackMatrix() const;

		/// <summary>
		/// Returns the value at the end of the data
		/// </summary>
		/// <returns>RTTI Pointer</returns>
		RTTI* BackPointer() const;
#pragma endregion

#pragma region Remove
		/// <summary>
		/// Remove int value, if value does not exist, returns false.
		/// </summary>
		/// <param name="value"> - value to remove</param>
		/// <returns>True or False</returns>
		bool Remove(int value);
		
		/// <summary>
		/// Remove float value, if value does not exist, returns false.
		/// </summary>
		/// <param name="value"> - value to remove</param>
		/// <returns>True or False</returns>
		bool Remove(float value);
	

		/// <summary>
		/// Remove string value, if value does not exist, returns false.
		/// </summary>
		/// <param name="value"> - value to remove</param>
		/// <returns>True or False</returns>
		bool Remove(const std::string& value);
		
		/// <summary>
		/// Remove vector value, if value does not exist, returns false.
		/// </summary>
		/// <param name="value"> - value to remove</param>
		/// <returns>True or False</returns>
		bool Remove(glm::vec4 value);

		/// <summary>
		/// Remove matrix value, if value does not exist, returns false.
		/// </summary>
		/// <param name="value"> - value to remove</param>
		/// <returns>True or False</returns>
		bool Remove(glm::mat4 value);
		
		/// <summary>
		/// Remove pointer value, if value does not exist, returns false.
		/// </summary>
		/// <param name="value"> - value to remove</param>
		/// <returns>True or False</returns>
		bool Remove(RTTI* value);

		/// <summary>
		/// Remove datum value at specified index
		/// </summary>
		/// <param name="index"> - position to remove at</param>
		/// <returns>True or False</returns>
		bool RemoveAt(size_type index);
#pragma endregion

#pragma region IndexOf
		/// <summary>
		/// Searches for the value and returns the index it is located at. If value is not found, then it returns size.
		/// </summary>
		/// <param name="value"> - Value to search for</param>
		/// <returns>Index of searched item</returns>
		size_type IndexOf(const int value);

		/// <summary>
		/// Searches for the value and returns the index it is located at. If value is not found, then it returns size.
		/// </summary>
		/// <param name="value"> - Value to search for</param>
		/// <returns>Index of searched item</returns>
		size_type IndexOf(const float value);

		/// <summary>
		/// Searches for the value and returns the index it is located at. If value is not found, then it returns size.
		/// </summary>
		/// <param name="value"> - Value to search for</param>
		/// <returns>Index of searched item</returns>
		size_type IndexOf(const std::string& value);

		/// <summary>
		/// Searches for the value and returns the index it is located at. If value is not found, then it returns size.
		/// </summary>
		/// <param name="value"> - Value to search for</param>
		/// <returns>Index of searched item</returns>
		size_type IndexOf(const glm::vec4 value);

		/// <summary>
		/// Searches for the value and returns the index it is located at. If value is not found, then it returns size.
		/// </summary>
		/// <param name="value"> - Value to search for</param>
		/// <returns>Index of searched item</returns>
		size_type IndexOf(const glm::mat4& value);

		/// <summary>
		/// Searches for the value and returns the index it is located at. If value is not found, then it returns size.
		/// </summary>
		/// <param name="value"> - Value to search for</param>
		/// <returns>Index of searched item</returns>
		size_type IndexOf(const RTTI* value);
#pragma endregion
		private:
			friend class Attributed;
			void SetStorage(DatumTypes type, void* data, size_type size);

			/// <summary>
			/// Pushes back all elements in initializer list. 
			/// </summary>
			/// <param name="list"> - initializer list</param>
			template<typename T>
			void PushBackInitializerList(std::initializer_list<T>& list);

			/// <summary>
			/// Pushes back default constructed value, used for growth during resize
			/// </summary>
			/// <param name="numberOfElements"> - number of elements to pushback</param>
			/// <param name="defaultValue"> - templated type to default construct</param>
			template<typename T>
			void PushBackDefault(size_t numberOfElements, T defaultValue);

			/// <summary>
			/// Pushes back scope value into table
			/// </summary>
			/// <param name="value"> - scope to push back</param>
			void PushBack(Scope& value);

			/// <summary>
			/// Finds index provided by given value
			/// </summary>
			/// <param name="datumData"> - data to search through</param>
			/// <param name="value"> - search value</param>
			/// <returns>Index of found value, otherwise, returns size if not found</returns>
			template<typename TData, typename TValue >
			size_type IndexOf(TData datumData, TValue& value);

			/// <summary>
			/// Union of data types
			/// </summary> 
			union DatumValues final
			{
				int* i;
				float* f;
				std::string* s;
				Scope** table;
				glm::vec4* v;
				glm::mat4* m;
				RTTI** p;
				void* vp;
				std::byte* b;
			};

			/// <summary>
			/// Datum value data
			/// </summary>
			DatumValues _data{ };

			/// <summary>
			/// Number of values stored in datum
			/// </summary>
			size_type _size{ 0 };

			/// <summary>
			/// Max number of values that can be stored in datum
			/// </summary>
			size_type _capacity{ 0 };

			/// <summary>
			/// Type datum represents
			/// </summary>
			DatumTypes _type{ DatumTypes::Unknown };
			
			/// <summary>
			/// Whether or not datum is external storage
			/// </summary>
			bool _isExternal{ false };

			/// <summary>
			/// size in bytes for each data type for datum
			/// </summary>
			static constexpr size_t _typeSizes[static_cast<int>(DatumTypes::End) + 1] =
			{
				0,
				sizeof(std::int32_t),
				sizeof(float),
				sizeof(std::string),
				sizeof(Scope*),
				sizeof(glm::vec4),
				sizeof(glm::mat4),
				sizeof(RTTI*),
			};
	};
}

#include "Datum.inl"