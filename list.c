/* 
 * list.c - list touch devices and their properties
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
#include <X11/extensions/XInput2.h>

int
main ()
{
	Display *dpy = XOpenDisplay(NULL);

	XIDeviceInfo *di;
	XIDeviceInfo *dev;
	XITouchClassInfo *class;
	int cnt;
	int printed;
	int i, j;

	puts("");

	di = XIQueryDevice(dpy, XIAllDevices, &cnt);
	for (i = 0; i < cnt; i ++)
	{
		dev = &di[i];

		printed = 0;
		for (j = 0; j < dev->num_classes; j ++)
		{
			class = (XITouchClassInfo*)(dev->classes[j]);

			if (class->type != XITouchClass)
				continue;

			if (!printed)
				printf("Device %d '%s'\n", dev->deviceid, dev->name),
				printed = 1;

	 		printf("\t%s touch\n\t%d touches\n",
				(class->mode == XIDirectTouch) ?  "DIRECT" : "DEPENDENT",
				class->num_touches
			);
		}
	}
	puts("");


	return 0;
}

