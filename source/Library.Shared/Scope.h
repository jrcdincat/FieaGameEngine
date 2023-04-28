#pragma once
#include "HashMap.h"
#include "Vector.h"
#include "Datum.h"
#include <gsl/gsl>
#include <stdexcept>

namespace FieaGameEngine
{
	/// <summary>
	/// Scope that represents a table of string keys and datums
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);
	public: 
		using size_type = std::size_t;

		/// <summary>
		/// Scope Constructor.
		/// </summary>
		/// <param name="capacity"> - capacity to start with</param>
		explicit Scope(size_type capacity = 11);

		/// <summary>
		/// Copy Constructor.
		/// </summary>
		/// <param name="other"> - Scope to copy</param>
		Scope(const Scope& other);

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs"> - Scope to copy</param>
		/// <returns>This scope</returns>
		Scope& operator=(const Scope& rhs);

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs"> - Scope to move into this scope</param>
		/// <returns>This scope</returns>
		Scope& operator=(Scope&& rhs) noexcept;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other"> - Scope to move into this scope</param>
		Scope(Scope&& other) noexcept;

		/// <summary>
		/// Deletes all memory allocated by this object.
		/// </summary>
		virtual void Clear(); 

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Heap allocates new scope
		/// </summary>
		/// <returns>Heap allocated scope</returns>
		[[nodiscard]] virtual gsl::owner<Scope*> Clone() const;

		/// <summary>
		/// Appends an empty datum with given key into scope
		/// </summary>
		/// <param name="key"> - Key to use to append</param>
		/// <returns>Datum that was created</returns>
		Datum& Append(const std::string key); 

		/// <summary>
		/// Append then create an empty scope.
		/// </summary>
		/// <param name="key"> - Key to use to append</param>
		/// <returns>Datum that was created</returns>
		Scope& AppendScope(const std::string key);

		/// <summary>
		/// This scope adopts the given scope and key to store the datum with. 
		/// Sets the scopes parent to this scope, and inserts the datum with given key.
		/// </summary>
		/// <param name="scope"> - Scope to adopt, MUST be heap allocated</param>
		/// <param name="key"> - key to use to insert the datum</param>
		void Adopt(Scope& scope, const std::string key);
		
		/// <summary>
		/// Removes orphan from parent and then sets its parent to nullptr.
		/// </summary>
		/// <returns>gsl owner scope pointer of the scope that is being orphaned</returns>
		gsl::owner<Scope*> Orphan();

		/// <summary>
		/// Returns this scope's parent.
		/// </summary>
		/// <returns>Scopes parent</returns>
		[[nodiscard]] Scope* Parent() const;

		/// <summary>
		/// Searches for key and returns the address associated with key.
		/// </summary>
		/// <param name="key"> - Key to search for</param>
		/// <returns>Datum pointer found, otherwise, nullptr</returns>
		[[nodiscard]] Datum* Find(const std::string key);

		/// <summary>
		/// Finds scope in the current level and returns the datum and index associated with it.
		/// </summary>
		/// <param name="scope"> - Scope to search for</param>
		/// <returns>Pair with index and datum associated with the found scope, otherwise, pair is index 0 and nullptr </returns>
		[[nodiscard]] std::pair<size_type, Datum*> FindContainedScope(const Scope& scope) const;
		
		/// <summary>
		/// Search for datum with given key and pass in output scope argument 
		/// </summary>
		/// <param name="key"> - Key to search for</param>
		/// <param name="foundScope"> - Output scope argument</param>
		/// <returns>Datum pointer</returns>
		[[nodiscard]] Datum* Search(const std::string key, Scope*& foundScope);

		/// <summary>
		/// Search for Datum with given key.
		/// </summary>
		/// <param name="key"> - Key to search for</param>
		/// <returns>Datum pointer</returns>
		[[nodiscard]] Datum* Search(const std::string key);

		/// <summary>
		/// Appends new datum at key.
		/// </summary>
		/// <param name="key">Key to append</param>
		/// <returns>Appended datum</returns>
		Datum& operator[](const std::string key);

		/// <summary>
		/// Returns datum at the given index.
		/// </summary>
		/// <param name="index"> - Position of datum in vector</param>
		/// <returns>Datum located at index</returns>
		Datum& operator[](size_type index) ;

		/// <summary>
		/// Returns found datum at the specified key address.
		/// </summary>
		/// <param name="key"></param>
		/// <returns></returns>
		const Datum& At(const std::string& key) const;

		/// <summary>
		/// Checks if scope is equal to the given scope.
		/// </summary>
		/// <param name="rhs"> - Scope to compare with</param>
		/// <returns>True or False</returns>
		[[nodiscard]] const bool operator==(const Scope& rhs) const;

		/// <summary>
		/// Checks if scope is not equal to the given scope.
		/// </summary>
		/// <param name="rhs"> - Scope to compare with</param>
		/// <returns>True or False</returns>
		[[nodiscard]] const bool operator!=(const Scope& rhs) const;

		/// <summary>
		/// Returns the size of the scope's vector.
		/// </summary>
		/// <returns>size of vector</returns>
		[[nodiscard]] size_type Size() const;

		/// <summary>
		/// Determine if this scope is an ancestor of the given scope.
		/// </summary>
		/// <param name="scope"> - Scope to check if ancestor of</param>
		/// <returns>True or False</returns>
		[[nodiscard]] const bool IsAncestorOf(const Scope& scope) const;

		/// <summary>
		/// Determine if this scope is a descendant of the given scope.
		/// </summary>
		/// <param name="scope"> - Scope to check if descendant of</param>
		/// <returns>True or False</returns>
		[[nodiscard]] const bool IsDescendantOf(const Scope& scope) const; 

		/// <summary>
		/// Returns whether scope contains given key.
		/// </summary>
		/// <param name="key"> - value to lookup in scope's hashmap</param>
		/// <returns>True or False</returns>
		[[nodiscard]] const bool ContainsKey(const std::string key) const;

		/// <summary>
		/// Get the key
		/// </summary>
		/// <returns>key</returns>
		const std::string& GetKey(size_type index) const;
	protected:
		/// <summary>
		/// Vector that stores the hashmap key datum pairs in the order they were added. 
		/// </summary>
		Vector<std::pair<const std::string, Datum>*> _orderVector;

		/// <summary>
		/// Hashmap that contains the key name and datum.
		/// </summary>
		HashMap<const std::string, Datum> _hashMap;
	private: 
		/// <summary>
		/// Scope parent of this scope instance.
		/// </summary>
		Scope* _parent = nullptr;

		/// <summary>
		/// Deep copies a Scope
		/// </summary>
		void DeepCopy(const Scope& other);
	};
}

#include "Scope.inl"