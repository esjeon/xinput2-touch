/* 
 * event.c - show touch events 
 *
 * Copyright © 2013 Eon S. Jeon <esjeon@live.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>

int
main ()
{
	Display *dpy = XOpenDisplay(NULL);
	int scr = DefaultScreen(dpy);

	int xi_opcode;
	int devid;
	Window win;

	/* check XInput extension */
	{
		int ev;
		int err;

		if (!XQueryExtension(dpy, "XInputExtension", &xi_opcode, &ev, &err)) {
			printf("X Input extension not available.\n");
			return 1;
		}
	}

	/* check the version of XInput */
	{
		int rc;
		int major = 2;
		int minor = 3;

		rc = XIQueryVersion(dpy, &major, &minor);
		if (rc != Success)
		{
			printf("No XI2 support. (%d.%d only)\n", major, minor);
			exit(1);
		}
	}

	/* create window */
	{
		XSetWindowAttributes attr = {
			.background_pixel = 0,
			.event_mask = KeyPressMask
		};

		win = XCreateWindow(dpy, RootWindow(dpy, scr),
				0, 0, 100, 100, 0,
				DefaultDepth(dpy, scr),
				InputOutput,
				DefaultVisual(dpy, scr),
				CWEventMask | CWBackPixel,
				&attr);

		XMapWindow(dpy, win);
		XSync(dpy, False);
	}

	/* select device */
	{
		XIDeviceInfo *di;
		XIDeviceInfo *dev;
		XITouchClassInfo *class;
		int cnt;
		int i, j;

		di = XIQueryDevice(dpy, XIAllDevices, &cnt);
		for (i = 0; i < cnt; i ++)
		{
			dev = &di[i];
			for (j = 0; j < dev->num_classes; j ++)
			{
				class = (XITouchClassInfo*)(dev->classes[j]);
				if (class->type != XITouchClass)
				{
					devid = dev->deviceid;
					goto STOP_SEARCH_DEVICE;
				}
			}
		}
STOP_SEARCH_DEVICE:
		XIFreeDeviceInfo(di);
	}

	/* select events to listen */
	{
		XIEventMask mask = {
			.deviceid = devid, //XIAllDevices,
			.mask_len = XIMaskLen(XI_TouchEnd)
		};
		mask.mask = (unsigned char*)calloc(3, sizeof(char));
		XISetMask(mask.mask, XI_TouchBegin);
		XISetMask(mask.mask, XI_TouchUpdate);
		XISetMask(mask.mask, XI_TouchEnd);

		XISelectEvents(dpy, win, &mask, 1);

		free(mask.mask);
	}


	XFlush(dpy);

	/* main loop */
	while(1) {
		XEvent ev;
		XGenericEventCookie *cookie = &ev.xcookie; // hacks!
		XIDeviceEvent *devev;

		XNextEvent(dpy, &ev);
		if (XGetEventData(dpy, cookie)) // extended event
		{
			// check if this belongs to XInput
			if(cookie->type == GenericEvent && cookie->extension == xi_opcode)
			{
				static int last = -1;

				devev = cookie->data;
				switch(devev->evtype) {
				case XI_TouchBegin:
					if (last != devev->evtype)
						fprintf(stderr, "TouchBegin\n");
					fprintf(stderr, "\t%X\t(%f, %f)\n", devev->detail,
						devev->root_x, devev->root_y);
					last = devev->evtype;
					break;
				case XI_TouchUpdate:
					if (last != devev->evtype)
						fprintf(stderr, "TouchBegin\n");
					fprintf(stderr, "\t%X\t(%f, %f)\n", devev->detail,
						devev->root_x, devev->root_y);
					last = devev->evtype;
					break;
				case XI_TouchEnd:
					if (last != devev->evtype)
						fprintf(stderr, "TouchEnd\n");
					fprintf(stderr, "\t%X\t(%f, %f)\n", devev->detail,
						devev->root_x, devev->root_y);
					last = devev->evtype;
					break;
				}
			}
		}
		else // normal event
		{
			if (ev.type == KeyPress)
				break;
		}
	}

	return 0;
}
