/*
 * Copyright (c) 2022 Brian Callahan <bcallah@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qoi.h"

int
main(int argc, char *argv[])
{
	Display *disp;
	Window win;
	XImage *ximg;
	XEvent e;
	qoi_desc desc;
	uint64_t i;
	uint32_t *pixels;
	int scr;

	if (argc != 2) {
		(void) fputs("usage: dqoi file.qoi\n", stderr);

		return 1;
	}

	if ((pixels = qoi_read(argv[1], &desc, 4)) == NULL) {
		(void) fprintf(stderr, "dqoi: error: read failed: %s\n",
		    argv[1]);

		return 1;
	}

	if (desc.width == 0 || desc.height == 0) {
		(void) fprintf(stderr, "dqoi: error: invalid image: %s\n",
		    argv[1]);

		return 1;
	}

	for (i = 0; i < desc.width * desc.height; i++) {
		pixels[i] = (pixels[i] >> 24) << 24 |
		    (pixels[i] & 0xff) << 16 |
		    ((pixels[i] >> 8) & 0xff) << 8 |
		    ((pixels[i] >> 16) & 0xff);
	}

	if ((disp = XOpenDisplay(NULL)) == NULL) {
		(void) fputs("dqoi: error: could not open display\n", stderr);

		return 1;
	}

	scr = DefaultScreen(disp);
	win = XCreateSimpleWindow(disp, RootWindow(disp, scr), 0, 0,
	    desc.width, desc.height, 0, 0, 0);

	XStoreName(disp, win, argv[1]);

	ximg = XCreateImage(disp, DefaultVisual(disp, scr),
	    DefaultDepth(disp, scr), ZPixmap, 0, (char *) pixels, desc.width,
	    desc.height, 8, 0);

	XSelectInput(disp, win, ExposureMask | KeyPressMask);
	XMapWindow(disp, win);

	for (;;) {
		XNextEvent(disp, &e);

		if (e.type == Expose) {
			XPutImage(disp, win, DefaultGC(disp, scr), ximg, 0, 0,
			    0, 0, desc.width, desc.height);
		}

		if (e.type == KeyPress)
			break;
	}

	XCloseDisplay(disp);

	free(pixels);

	return 0;
}