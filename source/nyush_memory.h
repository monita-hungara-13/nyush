#pragma once

#include "nyush_types.h"

inline void zero(void* in_mem, nyush::size in_size)
{
	char* bytes = (char*)in_mem;
	while (in_size--)
	{
		*bytes++ = 0;
	}
}
