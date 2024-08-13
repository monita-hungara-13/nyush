#include "nyush.h"

#include <windows.h>

void __stdcall platform_entry()
{
	int result = nyush::loop();
	ExitProcess(result);
}
