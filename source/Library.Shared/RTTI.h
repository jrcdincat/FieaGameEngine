#pragma once

#include <cstddef>
#include <string>

// midterm question:*******
// What is an abstract base class?
// Can never instantiate one, it is only instantiable through derived class
// abstract function is one where it is a pure virtual function
// abstract bas class has at least one pure virtual function
// ********

// marked protected in base class, then it can be accessed by the derive class, but not accessed outside of derived class
// marked public can be accessed through derived class
// marked private only accessible in base class not derive class
// 
namespace FieaGameEngine
{
	class RTTI
	{
	public:
		using IdType = std::size_t;

		RTTI() = default;
		RTTI(const RTTI&) = default;
		RTTI& operator=(const RTTI&) = default;
		RTTI(RTTI&&) noexcept = default;
		RTTI& operator=(RTTI&&) noexcept = default;
		virtual ~RTTI() = default;

		// All instances of the same type that are derived from RTTI will share the same TypeIdInstance 
		// Every single type has a unique type id
		virtual IdType TypeIdInstance() const = 0; 
		virtual bool Is(IdType) const;

		template <typename T>
		T* As();

		template <typename T>
		const T* As() const;

		virtual std::string ToString() const;
		virtual bool Equals(const RTTI* rhs) const;
	};
}

//
// If you inherit from a class and you don't specify public, it will automatically be privately be inherited
// public inheritance means everyone knows that it inherits from this class, private means only we know, If it is
// public inherited the inheritance is expose to everyone else.
// 
// unit test file 
// class foo final : public FieaGameEngine::RTTI
// RTTI_Declarations(Foo, FieaGameEngine::RTTI) - this is how to invoke the RTTI macro below
// \ at end is immediately followed by a newline so that it takes the next line and places it in a new line
// 
// Remember to use override, else it will make a completely separte version instead of overriding
// Can also mark overriden function  final to end its ability to be derived by future derived classes
// 
// RTTI_Definitions should have macro specified in cpp file. ex. in Foo.cpp
// What it does is it takes the type specified through cpp file and puts it everywhere where _typeId is because that type is the &Type
// Do this with non templated types
//
// Datum will have an array of RTTI pointers
//
#define RTTI_DECLARATIONS(Type, ParentType)																							\
	public:																													\
		static FieaGameEngine::RTTI::IdType TypeIdClass() { return _typeId; }												\
		FieaGameEngine::RTTI::IdType TypeIdInstance() const override { return TypeIdClass(); }								\
		bool Is(FieaGameEngine::RTTI::IdType id) const override { return (id == _typeId ? true : ParentType::Is(id)); }		\
	private:																												\
		static const FieaGameEngine::RTTI::IdType _typeId

#define RTTI_DEFINITIONS(Type)																								\
	const FieaGameEngine::RTTI::IdType Type::_typeId = reinterpret_cast<FieaGameEngine::RTTI::IdType>(&Type::_typeId)
	

#include "RTTI.inl"