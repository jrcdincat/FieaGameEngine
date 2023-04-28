#pragma once
#include <cassert>
#include "HashMap.h"
#include "Vector.h"
#include "Signature.h"

namespace FieaGameEngine
{
	using size_type = std::size_t;

	/// <summary>
	/// Type manager that stores signatures
	/// </summary>
	class TypeManager final
	{
	public:
		/// <summary>
		/// Gets the static type manager instance
		/// </summary>
		/// <returns>static type manager instance</returns>
		static TypeManager* Instance();

		/// <summary>
		/// Instantiate the static type manager instance.
		/// </summary>
		static void CreateInstance();

		/// <summary>
		/// Delete the static type manager instance.
		/// </summary>
		static void DestroyInstance();

		/// <summary>
		/// Insert into hash map of signatures with given pair.
		/// </summary>
		/// <param name="pair"> - pair to be inserted</param>
		/// <returns>True or False</returns>
		static bool Insert(std::pair<RTTI::IdType, Vector<Signature>> pair);

		/// <summary>
		/// Checks to see if signatures are registered with the current typeId
		/// </summary>
		[[nodiscard]] const static bool ContainsTypeId(const RTTI::IdType typeId); 

		/// <summary>
		/// Add signatures with associated typeID, including the parent classes signatures. Parents signatures must already 
		/// be registered before derived classes.
		/// </summary>
		/// <param name="typeId"> - derived classes typeId</param>
		/// <param name="signatures"> - derived classes returned signatures to be added</param>
		/// <param name="parentTypeId"> - parent classes typeId</param>
		static void Add(RTTI::IdType typeId, Vector<Signature> signatures, RTTI::IdType parentTypeId);

		/// <summary>
		/// Gets the signatures for specified type id
		/// </summary>
		/// <param name="typeId"> - typeId to lookup to get signatures of</param>
		/// <returns>Reference to a vector of signatures</returns>
		[[nodiscard]] Vector<Signature>& GetSignaturesOfTypeId(size_type typeId);
	private:

		/// <summary>
		/// Static type manager instance.
		/// </summary>
		inline static TypeManager* _instance{ nullptr };

		/// <summary>
		/// Hash map of signatures.
		/// </summary>
		HashMap<size_type, Vector<Signature>> _signatures;
	};

	/// <summary>
	/// Registers the signatures with given derived type and parent type by calling add
	/// </summary>
	template <typename Type, typename ParentType>
	void RegisterType()
	{
		TypeManager::Add(Type::TypeIdClass(), Type::Signatures(), ParentType::TypeIdClass());
	}
}