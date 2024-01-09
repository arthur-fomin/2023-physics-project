#if (defined(WIN32) || defined(_WIN32))
#include <windows.h>
#include <commdlg.h>
#endif

#include "ndFileBrowser.h"

static void strtolwr(char* const string)
{
	for (char * cp = string; *cp; ++cp)
	{
		if ((*cp >= 'A') && (*cp <= 'Z'))
		{
			*cp += 'a' - 'A';
		}
	}
}

