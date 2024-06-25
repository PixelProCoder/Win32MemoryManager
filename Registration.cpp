#include "Registration.h"

namespace Azul
{
	Registration::Registration()
	{
		set.reserve(ReserveSize);
	}

	Registration::~Registration()
	{
	}

	bool Registration::Find(void* p)
	{
		return set.find(p) != set.end();
	}

	void Registration::Register(void* p)
	{
		set.insert(p);
	}

	void Registration::UnRegister(void* p)
	{
		set.erase(p);
	}
}
