#include "pch.h"
#include "Scope.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope);
	Scope::Scope(size_type capacity)
	{
		_orderVector = Vector<std::pair<const std::string, Datum>*>{ capacity };
	}

	Scope::Scope(const Scope& other)
	{
		DeepCopy(other);
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		Clear();
		DeepCopy(rhs);
		return *this;
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept
	{
		Clear();
		_hashMap.~HashMap();
		_hashMap = std::move(rhs._hashMap);
		_orderVector = std::move(rhs._orderVector);
		for (size_type index = 0; index < _orderVector.size(); ++index)
		{
			Datum& datum = _orderVector[index]->second;
			if (datum.Type() == Datum::DatumTypes::Table && datum._size > 0)
			{
				datum.GetScope()._parent = this;
			}
		}

		if (rhs._parent != nullptr)
		{
			rhs.Orphan();
			delete& rhs;
		}
		return *this;
	}

	Scope::Scope(Scope&& other) noexcept :
		_hashMap{ std::move(other._hashMap) }, _orderVector{ std::move(other._orderVector) }
	{
		for (size_type index = 0; index < _orderVector.size(); ++index)
		{
			Datum& datum = _orderVector[index]->second;
			if (datum.Type() == Datum::DatumTypes::Table && datum._size > 0)
			{
				datum.GetScope()._parent = this;
			}
		}
		if (other._parent != nullptr)
		{
			other.Orphan();
			delete& other;
		}
	}

	Scope::~Scope()
	{
		Clear();
	}

	void Scope::Clear()
	{
		for (size_type index = 0; index < _orderVector.size(); ++index)
		{
			auto& [key, datum] = *_orderVector[index];

			if (datum.Type() == Datum::DatumTypes::Table)
			{
				for (size_type i = 0; i < datum.Size(); ++i)
				{
					auto& childScope = datum.GetScope(i);
					childScope._parent = nullptr;
					delete &childScope;				
				}
			}
		}
		_hashMap.Clear();
		_orderVector.Clear();
		_orderVector.ShrinkToFit();
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return gsl::owner<Scope*>(new Scope(*this));
	}

	Datum& Scope::Append(const std::string key)
	{
		if (key.empty())
		{
			throw std::invalid_argument("Cannot append empty key name.");
		}

		auto returnedPair = _hashMap.Insert({ key, Datum{} });
		if (returnedPair.second)
		{
			_orderVector.push_back(&*returnedPair.first);
		}

		return returnedPair.first->second;
	}

	Scope& Scope::AppendScope(const std::string key)
	{
		Datum& datum = Append(key);
		if (datum.Type() == Datum::DatumTypes::Unknown)
		{
			datum.SetType(Datum::DatumTypes::Table);
		}
		Scope* scope = new Scope{};

		scope->_parent = this;
		datum.PushBack(scope);
		return *scope;
	}

	void Scope::Adopt(Scope& scope, const std::string key)
	{
		if (&scope == this)
		{
			throw std::runtime_error("Scope adoption of oneself is not allowed.");
		}
		if (scope.IsAncestorOf(*this))
		{
			throw std::runtime_error("Adoption of a scope ancestor is not allowed.");
		}
		else if (key.empty())
		{
			throw std::invalid_argument("Cannot adopt a scope with empty key name.");
		}

		Datum& datum = Append(key);
		if (datum.Type() != Datum::DatumTypes::Table && datum.Type() != Datum::DatumTypes::Unknown)
		{
			throw std::runtime_error("Adoption of a scope ancestor is not allowed.");
		}

		scope.Orphan();
		scope._parent = this;
		datum.PushBack(scope);
	}

	gsl::owner<Scope*> Scope::Orphan()
	{
		if (_parent != nullptr)
		{
			auto [index, datum] = _parent->FindContainedScope(*this);
			if (datum != nullptr)
			{
				datum->RemoveAt(index);
				_parent = nullptr;
			}

			return gsl::owner<Scope*>(this);
		}

		return nullptr;
	}

	Scope* Scope::Parent() const
	{
		return _parent;
	}

	Datum* Scope::Find(const std::string key)
	{
		return _hashMap.ContainsKey(key) ? &_hashMap.Find(key)->second : nullptr;
	}

	std::pair<Scope::size_type, Datum*> Scope::FindContainedScope(const Scope& scope) const
	{
		for (size_type index = 0; index < _orderVector.size(); ++index)
		{
			auto& [key, datum] = *_orderVector[index];
			if (datum.Type() == Datum::DatumTypes::Table)
			{
				for (size_t datumIndex = 0; datumIndex < datum.Size(); ++datumIndex)
				{
					Scope& childScope = datum.GetScope(datumIndex);
					if (&childScope == &scope)
					{
						return { datumIndex, &datum };
					}
				}
			}
		}
		return { 0, nullptr };
	}

	Datum* Scope::Search(const std::string key, Scope*& foundScope)
	{
		auto returnedIterator = _hashMap.Find(key);
		if (returnedIterator == _hashMap.end())
		{
			return _parent == nullptr ? nullptr : _parent->Search(key);
		}
		else
		{
			foundScope = this;
			return &returnedIterator->second;
		}
	}

	Datum* Scope::Search(const std::string key)
	{
		Scope* foundScope;
		return Search(key, foundScope);
	}

	Datum& Scope::operator[](const std::string key)
	{
		return Append(key);
	}

	Datum& Scope::operator[](size_type index)
	{
		if (index >= _orderVector.size())
		{
			throw std::invalid_argument("Index out of bounds.");
		}

		return _orderVector[index]->second;
	}

	const Datum& Scope::At(const std::string& key) const
	{
		return _hashMap.At(key);
	}

	const bool Scope::operator==(const Scope& rhs) const
	{
		if (this->Size() != rhs.Size())
		{
			return false;
		}

		for (size_type index = 0; index < rhs.Size(); ++index)
		{
			if (_orderVector[index]->first != rhs._orderVector[index]->first ||
				_orderVector[index]->second != rhs._orderVector[index]->second)
			{
				return false;
			}
		}

		return true;
	}

	const bool Scope::operator!=(const Scope& rhs) const
	{
		return !operator==(rhs);
	}


	const bool Scope::IsAncestorOf(const Scope& scope) const
	{
		if (scope._parent == nullptr)
		{
			return false;
		}

		if (scope._parent != this)
		{
			return IsAncestorOf(*scope._parent);
		}

		return true;
	}

	const bool Scope::IsDescendantOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}

	const bool Scope::ContainsKey(const std::string key) const
	{
		return _hashMap.ContainsKey(key);
	}

	void Scope::DeepCopy(const Scope& other)
	{
		_orderVector.Reserve(other._orderVector.size());

		for (size_type index = 0; index < other.Size(); ++index)
		{
			auto& [key, existingDatum] = *other._orderVector[index];
			Datum& newDatum = Append(key);
			if (existingDatum.Type() == Datum::DatumTypes::Table)
			{
				newDatum.SetType(Datum::DatumTypes::Table);
				newDatum.Reserve(existingDatum.Size());
				for (size_type tableIndex = 0; tableIndex < existingDatum._size; ++tableIndex)
				{
					Scope* scopeCopy = existingDatum.GetScope().Clone();
					newDatum.PushBack(scopeCopy);
				}
			}
			else
			{
				newDatum = existingDatum;
			}
		}
	}

	const std::string& Scope::GetKey(size_type index) const
	{
		return _orderVector.At(index)->first;
	}
}