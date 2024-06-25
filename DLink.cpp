#include "DLink.h"

namespace Azul
{
	DLink::DLink()
		: pNext{ nullptr }
		, pPrev{ nullptr }
	{
	}

	DLink::DLink(const DLink&)
		: pNext{ nullptr }
		, pPrev{ nullptr }
	{
	}

	DLink::~DLink()
	{
		// do nothing
	}

	DLink& DLink::operator = (const DLink &)
	{
		this->Reset();
		return *this;
	}

	void DLink::Reset()
	{
		this->pNext = nullptr;
		this->pPrev = nullptr;
	}

}
