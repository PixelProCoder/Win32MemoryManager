#ifndef MEM_SAP_H
#define MEM_SAP_H

#include "Mem.h"

namespace Azul
{
	class MemSAP final : public Mem
	{
	public:
		void SAPSetInfo(const Info& info);
		void SAPRegisterPtr(void* ptr);
		void SAPUnRegisterPtr(void* ptr);
		void SAPSetGlobalHead(Block* block);
	};
}

#endif