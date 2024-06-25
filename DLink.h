#ifndef DLINK_H
#define DLINK_H

namespace Azul
{
	class DLink final
	{
	public:
		// Big Four
		DLink();
		DLink(const DLink &);
		DLink &operator = (const DLink &);
		DLink(DLink &&) =  delete;
		DLink &operator = (DLink &&) = delete;
		~DLink();

		void Reset();

	public:
		DLink *pNext;
		DLink *pPrev;
	};
}

#endif
