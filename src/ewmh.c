/* Reserved for EWMH */

#include <stdbool.h>
#include <X11/Xatom.h>

/* Add atoms */

void setup_hints(void)
{
	Atom utf8_string = XInternAtom(d, "UTF8_STRING", false);
	Atom netwmname = XInternAtom(d, "_NET_WM_NAME", false);
	XChangeProperty(d, root, netwmname, utf8_string, 8,
			PropModeReplace, (unsigned char*)"Shard", 5);
}



