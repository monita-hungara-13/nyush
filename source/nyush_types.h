#pragma once

namespace nyush
{

using byte = unsigned char;

using int8 = signed char;
using int16 = short;
using int32 = int;
using int64 = long long;

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

using size = decltype(sizeof(0));

struct ivec2
{
	int32 x;
	int32 y;
};

} // namespace nyush
