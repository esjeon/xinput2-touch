
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

