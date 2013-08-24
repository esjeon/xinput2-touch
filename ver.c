
#include <stdio.h>
#include <X11/extensions/XInput2.h>

int
main ()
{
	Display *dpy;

	int rc;
	int major = 2;
	int minor = 2;

	dpy = XOpenDisplay(NULL);


	rc = XIQueryVersion(dpy, &major, &minor);
	if (rc == Success)
		printf("XI2 supported. (%d.%d)\n", major, minor);
	else if (rc == BadRequest)
		printf("No XI2 support. (%d.%d only)\n", major, minor);
	else
		printf("Internal error\n");


	return 0;
}

