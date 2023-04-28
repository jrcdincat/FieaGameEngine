#include "pch.h"
#include "Attributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(const Attributed& other) : Scope{ other }
	{
		Populate(other.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& other) noexcept : Scope{ std::move(other) }
	{
		Populate(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		Scope::operator=(rhs);
		Populate(rhs.TypeIdInstance());
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		Scope::operator=(std::move(rhs));
		Populate(rhs.TypeIdInstance());
		return *this;
	}

	bool Attributed::IsAttribute(std::string& name)
	{
		return ContainsKey(name);
	}

	bool Attributed::IsPrescribedAttribute(std::string& name)
	{
		Vector<Signature>& signatures = Signatures();
		for (auto& value : signatures)
		{
			if (value._name == name)
			{
				return true;
			}
		}

		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(std::string& name)
	{
		return (IsAttribute(name) && !IsPrescribedAttribute(name));
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		if (name.empty())
		{
			throw std::invalid_argument("Name cannot be an empty string.");
		}
		if (Find(name) != nullptr)
		{
			throw std::invalid_argument("Name already exists as an attribute.");
		}

		return Append(name);
	}

	std::pair<size_type, size_type> Attributed::Attributes()
	{	
		return { 0, Size() };
	}

	std::pair<size_type, size_type> Attributed::PrescribedAttributes()
	{
		return { 0, Signatures().size() + 1 };
	}

	std::pair<size_type, size_type> Attributed::AuxiliaryAttributes()
	{
		return { Signatures().size() + 1, Size() };
	}

	Vector<Signature>& Attributed::Signatures()
	{
		return TypeManager::Instance()->GetSignaturesOfTypeId(TypeIdInstance());
	}

	Attributed::Attributed(IdType typeId)
	{
		Populate(typeId);
	}

	void Attributed::Populate(IdType typeId)
	{
		Datum& datum = Append("this"s);
		datum = this;

		auto& signatures = TypeManager::Instance()->GetSignaturesOfTypeId(typeId);		
		for (size_type index = 0; index < signatures.size(); ++index)
		{	
			const Signature& signature = signatures[index];
			Datum& newDatum = Append(signature._name);

			if (signature._type != Datum::DatumTypes::Table)
			{
				newDatum.SetStorage(signature._type, reinterpret_cast<std::byte*>(this) + signature._offset, signature._size);
			}
			else
			{
				newDatum.SetType(Datum::DatumTypes::Table);
			}
		}
	}
}