#include "MemSAP.h"

void Azul::MemSAP::SAPSetInfo(const Info& info)
{
	mInfo = info;
}

void Azul::MemSAP::SAPRegisterPtr(void* ptr)
{
	RegisterPtr(ptr);
}

void Azul::MemSAP::SAPUnRegisterPtr(void* ptr)
{
	UnRegisterPtr(ptr);
}

void Azul::MemSAP::SAPSetGlobalHead(Block* block)
{
	SetGlobalHead(block);
}
