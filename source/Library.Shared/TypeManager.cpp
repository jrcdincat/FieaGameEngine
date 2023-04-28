#include "pch.h"
#include "TypeManager.h"
#include "Attributed.h"

namespace FieaGameEngine
{
	TypeManager* TypeManager::Instance()
	{
		return _instance;
	}

	void TypeManager::CreateInstance()
	{
		assert(_instance == nullptr);
		_instance = new TypeManager;
	}

	void TypeManager::DestroyInstance()
	{
		delete _instance;
	}

	bool TypeManager::Insert(std::pair<RTTI::IdType, Vector<Signature>> pair)
	{
		auto [iterator, isInserted] =_instance->_signatures.Insert(pair);
		return isInserted;
	}

	const bool TypeManager::ContainsTypeId(const RTTI::IdType typeId)
	{
		return _instance->_signatures.ContainsKey(typeId);
	}

	void TypeManager::Add(RTTI::IdType typeId, Vector<Signature> signatures, RTTI::IdType parentTypeId)
	{
		Vector<Signature> signaturesToInsert;
		
		if (parentTypeId != Attributed::TypeIdClass())
		{
			assert(ContainsTypeId(parentTypeId));
			signaturesToInsert = _instance->GetSignaturesOfTypeId(parentTypeId);
			
			for (auto signature : signatures)
			{
				signaturesToInsert.push_back(signature);
			}
		}
		else
		{
			signaturesToInsert = std::move(signatures);
		}
		
		
		_instance->Insert({ typeId, signaturesToInsert });
	}

	Vector<Signature>& TypeManager::GetSignaturesOfTypeId(size_type typeId)
	{
		if (!ContainsTypeId(typeId))
		{
			throw std::invalid_argument("Invalid type ID. Type manager does not have signatures for given type ID");
		}
		return _signatures[typeId];
	}
}