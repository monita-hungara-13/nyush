#include "nyush_runtime.h"

extern "C"
{

#pragma function(memset)
	void* memset(void* in_destination, int in_value, size_t in_count)
	{
		char* bytes = (char*)in_destination;
		while (in_count--)
		{
			*bytes++ = (char)in_value;
		}
		return in_destination;
	}

// #pragma function(memcpy)
// 	void* memcpy(void* dest, const void* src, size_t count)
// 	{
// 		char* dest8 = (char*)dest;
// 		const char* src8 = (const char*)src;
// 		while (count--)
// 		{
// 			*dest8++ = *src8++;
// 		}
// 		return dest;
// 	}
//
// #pragma function(memcmp)
// 	int memcmp(const void* buf1, const void* buf2, size_t count)
// 	{
// 		if (!count)
// 			return (0);
//
// 		while (--count && *(char*)buf1 == *(char*)buf2)
// 		{
// 			buf1 = (char*)buf1 + 1;
// 			buf2 = (char*)buf2 + 1;
// 		}
//
// 		return (*((unsigned char*)buf1) - *((unsigned char*)buf2));
// 	}

}
