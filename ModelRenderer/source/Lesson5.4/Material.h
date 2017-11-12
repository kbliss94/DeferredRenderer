#pragma once

#include "pch.h"
//#include "Effect.h"

namespace Library
{
	class Material : public RTTI
	{
		RTTI_DECLARATIONS(Material, RTTI)

	public:
		Material();
		Material(const std::string& defaultTechniqueName);
		virtual ~Material();
	};
}