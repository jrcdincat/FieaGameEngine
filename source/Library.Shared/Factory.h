#pragma once
#include <memory>
#include <string>
#include "HashMap.h"
#include "gsl/gsl"

namespace FieaGameEngine
{
	/// <summary>
	/// Factory class that allows creating class instance through derived factories
	/// </summary>
	/// <typeparam name="T">Factory type</typeparam>
	template <typename T> 
	class Factory
	{
	public: 
		/// <summary>
		/// Finds factory by given class name
		/// </summary>
		/// <param name="name"> - Class name of factory</param>
		/// <returns>Pointer to the factory if found, otherwise, nullptr</returns>
		static const Factory* Find(const std::string& name);

		/// <summary>
		/// Create instance of class type with given name if factory exists
		/// </summary>
		/// <param name="name"> - class name to create</param>
		/// <returns>Class instance pointer if found, otherwise, nullptr</returns>
		static gsl::owner<T*> Create(const std::string& name);

		/// <summary>
		/// Defaulted default constructor
		/// </summary>
		Factory() = default; 

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name=""> - Factory to copy</param>
		Factory(const Factory&) = default;
		
		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name=""> - Factory to move</param>
		Factory(Factory&&) = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name=""> - Factory to copy</param>
		/// <returns>Factory reference</returns>
		Factory& operator=(const Factory&) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name=""> - Factory to move</param>
		/// <returns>Factory reference</returns>
		Factory& operator=(Factory&&) = default;

		/// <summary>
		/// Defaulted virtual destructor
		/// </summary>
		virtual ~Factory() = default;

		/// <summary>
		/// Returns the class name that was used to create factory
		/// </summary>
		/// <returns>String of class name</returns>
		[[nodiscard]] virtual const std::string ClassName() const = 0;

		/// <summary>
		/// Create derived class 
		/// </summary>
		/// <returns>Pointer to newly created class instance</returns>
		virtual gsl::owner<T*> Create() const = 0;

		/// <summary>
		/// Size of the factory map
		/// </summary>
		/// <returns>Number of factories in map</returns>
		static size_t Size();

		/// <summary>
		/// Add a factory to the factory map
		/// </summary>
		/// <param name="factoryToAdd"> - Factory to be added to the map</param>
		static void Add(std::unique_ptr<Factory>&& factoryToAdd);

		/// <summary>
		/// Clear the factory map
		/// </summary>
		static void Clear();

		/// <summary>
		/// Remove a factory from the factory map 
		/// </summary>
		/// <param name="name"> - factory name to remove</param>
		inline static void Remove(const std::string& name);

	protected: 
		/// <summary>
		/// Hashmap that contains all of the factories that have been added
		/// </summary>
		inline static HashMap<std::string, std::unique_ptr<const Factory>> _factoryMap;
	};
}

#include "Factory.inl"