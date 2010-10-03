#include "wm.h"

static void map_request(XEvent *e, Display *dpy)
{
    static XWindowAttributes wa;

    XMapRequestEvent *ev = &e->xmaprequest;

    if(!XGetWindowAttributes(dpy, ev->window, &wa))
    {
        return;
    }

    if(wa.override_redirect)
    {
        return;
    }

    XMoveResizeWindow(dpy, ev->window, 100, 100, 100, 100); /* some windows require this */
    XMapWindow(dpy, ev->window);
    XSetInputFocus(dpy, ev->window, RevertToPointerRoot, CurrentTime);
}

void wm_process_events(Display *dpy)
{
    XEvent ev;
    int num;

    XSync(dpy, False);
    num = XPending(dpy);
    while(num > 0 && !XNextEvent(dpy, &ev)) {
        if(ev.type == MapRequest)
        {
            map_request(&ev, dpy);
        }
        num--;
    }
}

void wm_start(Display *dpy, int top, int width, int height)
{
    XSetWindowAttributes wa;
    int screen;
    Window root;

    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);
    /* select for events */
    wa.event_mask = SubstructureRedirectMask|SubstructureNotifyMask|ButtonPressMask
                    |EnterWindowMask|LeaveWindowMask|StructureNotifyMask
                    |PropertyChangeMask;

    XChangeWindowAttributes(dpy, root, CWEventMask, &wa);
    XSelectInput(dpy, root, wa.event_mask);
}

void wm_stop()
{
}
