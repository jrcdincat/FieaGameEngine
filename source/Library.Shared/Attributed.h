#pragma once
#include "Scope.h"
#include "Signature.h"
#include <utility>
#include "TypeManager.h"

using namespace std::string_literals;

namespace FieaGameEngine
{
	using size_type = std::size_t;

	/// <summary>
	/// Attributed is a class that inherits from Scope. 
	/// </summary>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);
	public: 
		/// <summary>
		/// Default constructor deleted.
		/// </summary>
		Attributed() = delete;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="other"> - attributed class to copy</param>
		Attributed(const Attributed& other);
		
		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other"> - attributed class to move.</param>
		Attributed(Attributed&& other) noexcept;
		
		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs"> - attributed class to copy</param>
		/// <returns>Attributed reference</returns>
		Attributed& operator=(const Attributed& rhs);
		
		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs"> - attributed class to move</param>
		/// <returns>Attributed reference</returns>
		Attributed& operator=(Attributed&& rhs) noexcept;
		
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Attributed() = default;

		/// <summary>
		/// Check if the given attribute name is an attribute.
		/// </summary>
		/// <param name="name"> - attribute name</param>
		/// <returns>True or False</returns>
		[[nodiscard]] bool IsAttribute(std::string& name);
		
		/// <summary>
		/// Check if the given attribute name is a prescribed attribute.
		/// </summary>
		/// <param name="name"> - attribute name</param>
		/// <returns>True or False</returns>
		[[nodiscard]] bool IsPrescribedAttribute(std::string& name);
		
		/// <summary>
		/// Check if given attribute name is an auxiliary attribute.
		/// </summary>
		/// <param name="name"> - attribute name</param>
		/// <returns>True or False</returns>
		[[nodiscard]] bool IsAuxiliaryAttribute(std::string& name);

		/// <summary>
		/// Append auxiliary attribute.
		/// </summary>
		/// <param name="name"> - name of the attribute</param>
		/// <returns>Newly created datum of that attribute</returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/// <summary>
		/// Return all attributes stored.
		/// </summary>
		/// <returns>Range (first, last)</returns>
		[[nodiscard]] std::pair<size_type, size_type> Attributes();
		
		/// <summary>
		/// Returns the range where prescribed attributes are located within the order vector 
		/// </summary>
		/// <returns>Range (first, last)</returns>
		[[nodiscard]] std::pair<size_type, size_type> PrescribedAttributes();

		/// <summary>
		/// Returns the range where auxiliary attributes are located within the order vector they are stored in.
		/// </summary>
		/// <returns>Range (first, last)</returns>
		[[nodiscard]] std::pair<size_type, size_type> AuxiliaryAttributes();

	protected:
		/// <summary>
		/// Gets signatures for derived class
		/// </summary>
		/// <returns>Vector of signatures</returns>
		virtual Vector<Signature>& Signatures();

		/// <summary>
		/// Protected Constructor
		/// </summary>
		/// <param name="typeId"> - type ID of derived class</param>
		explicit Attributed(IdType typeId);

	private:

		/// <summary>
		// Mechanism to populate Scope with prescribed members
		/// </summary>
		/// <param name="typeId"> - type ID to get signatures for</param>
		void Populate(IdType typeId); 
	};
}