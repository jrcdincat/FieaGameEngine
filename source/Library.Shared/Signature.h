#pragma once
#include "Datum.h"

namespace FieaGameEngine
{
	using size_type = std::size_t;

	/// <summary>
	/// Signature to be stored in type manager
	/// </summary>
	struct Signature final
	{
		/// <summary>
		/// Name
		/// </summary>
		std::string _name;

		/// <summary>
		/// Type of datum
		/// </summary>
		Datum::DatumTypes _type;

		/// <summary>
		/// Number of elements
		/// </summary>
		size_type _size;

		/// <summary>
		/// Offset from base data address
		/// </summary>
		size_type _offset;
	};
}