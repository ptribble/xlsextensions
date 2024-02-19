/*
 * xlsextensions.c Peter C. Tribble 2/12/1999
 * print information about available extensions. Essentially a
 * stripped down version of xdpyinfo. Original copyright notice:
 * $TOG: xdpyinfo.c /main/35 1998/02/09 13:57:05 kaleb $
 * 
 * xdpyinfo - print information about X display connecton
 *
 * 
Copyright 1988, 1998  The Open Group

All Rights Reserved.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/multibuf.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/sync.h>
#include <X11/extensions/Xdbe.h>
#include <X11/extensions/record.h>
#ifdef MITSHM
#include <X11/extensions/XShm.h>
#endif
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>

char *ProgramName;

static int StrCmp(a, b)
    char **a, **b;
{
    return strcmp(*a, *b);
}

void
print_extension_info (dpy)
    Display *dpy;
{
    int n = 0;
    char **extlist = XListExtensions (dpy, &n);

    if (extlist) {
	register int i;
	int opcode, event, error;

	qsort(extlist, n, sizeof(char *), StrCmp);
	for (i = 0; i < n; i++) {
		printf ("    %s\n", extlist[i]);
	}
	/* do not free, Xlib can depend on contents being unaltered */
	/* XFreeExtensionList (extlist); */
    }
}

static void usage ()
{
    fprintf (stderr, "usage:  %s [options]\n", ProgramName);
    fprintf (stderr, "-display displayname\tserver to query\n");
    exit (1);
}

int main (argc, argv)
    int argc;
    char *argv[];
{
    Display *dpy;			/* X connection */
    char *displayname = NULL;		/* server to contact */
    int i;				/* temp variable:  iterator */

    ProgramName = argv[0];

    for (i = 1; i < argc; i++) {
	char *arg = argv[i];
	int len = strlen(arg);
	
	if (!strncmp("-display", arg, len)) {
	    if (++i >= argc) usage ();
	    displayname = argv[i];
	} else
	    usage ();
    }

    dpy = XOpenDisplay (displayname);
    if (!dpy) {
	fprintf (stderr, "%s:  unable to open display \"%s\".\n",
		 ProgramName, XDisplayName (displayname));
	exit (1);
    }

    print_extension_info (dpy);

    XCloseDisplay (dpy);
    exit (0);
}
