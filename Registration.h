#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <unordered_set>

namespace Azul
{
	class Registration final
	{
	public:
		static const unsigned int ReserveSize = 256;

	public:
		Registration();
		Registration(const Registration &) = delete;
		Registration &operator = (const Registration &) = delete;
		Registration(Registration &&) = delete;
		Registration &operator = (Registration &&) = delete;
		~Registration();

		bool Find(void* p);
		void Register(void* p);
		void UnRegister(void* p);

		std::unordered_set<void *> set;
	};
}

#endif
