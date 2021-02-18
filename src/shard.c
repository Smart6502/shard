/*
BSD 2-Clause License

Copyright (c) 2021, Smart6502
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <X11/Xlib.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <util.h>

#define win        (client *t=0, *c=list; c && t!=list->prev; t=c, c=c->next)
#define ws_save(W) {\
	ws_list[W] = list;\
	ws_info[W] = info;\
}
#define ws_sel(W)  {\
	list = ws_list[ws = W];\
	info = ws_info[ws = W];\
}
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#define sizewin(W, gx, gy, gw, gh) \
    XGetGeometry(d, W, &(Window){0}, gx, gy, gw, gh, \
                 &(unsigned int){0}, &(unsigned int){0})
#define mod_clean(mask) (mask & ~(numlock|LockMask) & \
        (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))

typedef struct {
    const char** com;
    const int i;
    const Window w;
} Arg;

struct key {
    unsigned int mod;
    KeySym keysym;
    void (*function)(const Arg arg);
    const Arg arg;
};

typedef struct client {
    struct client *next, *prev;
    int f, wx, wy;
    unsigned int ww, wh;
    Window w;
    bool locked;
} client;

typedef struct _wsinfo {
	int size;
	Window wins[15];
	Window master;
} wsinfo;

void focus(client *c);
void button_press(XEvent *e);
void button_release(XEvent *e);
void confreq(XEvent *e);
int getwnum(const Window w);
void init(void);
void mapreq(XEvent *e);
void keypress(XEvent *e);
void notify_enter(XEvent *e);
void notify_destroy(XEvent *e);
void notify_mapping(XEvent *e);
void notify_motion(XEvent *e);
void addwin(Window w);
void delwin(Window w);
void killwin(const Arg arg);
void centerwin(const Arg arg);
void tile(const Window w);
void deltile(const Window w);
void setup_hints(void);
void sizetiles(const Arg arg);
void swaptv(const Arg arg);
void updatetiles(void);
void win_fs(const Arg arg);
void win_to_ws(const Arg arg);
void win_prev(const Arg arg);
void win_next(const Arg arg);
void ws_go(const Arg arg);
static void run(const Arg arg);
void quit(const Arg arg);
void grabkeyb(Window root);
static int xerror() { return 0; }

static client       *list = {0}, *ws_list[10] = {0}, *cur;
static wsinfo	    info, ws_info[10];
static int          ws = 1, sw, sh, wx, wy, numlock = 0, restart = 0, running = 1;
static unsigned int ww, wh;

static Display		*d;
static XButtonEvent	mouse;
static Window		root;

#include <config.h>
#include <tiling.c>
#include <ewmh.c>
#include <events.c>

static void (*evhandler[LASTEvent])(XEvent *e) = {
    [ButtonPress]      = button_press,
    [ButtonRelease]    = button_release,
    [ConfigureRequest] = confreq,
    [KeyPress]         = keypress,
    [MapRequest]       = mapreq,
    [MappingNotify]    = notify_mapping,
    [DestroyNotify]    = notify_destroy,
    [EnterNotify]      = notify_enter,
    [MotionNotify]     = notify_motion
};

void focus(client *c) {
    cur = c;
    XSetInputFocus(d, cur->w, RevertToParent, CurrentTime);
}

void addwin(Window w) {
    client *c;

    if (!(c = (client *) calloc(1, sizeof(client))))
        exit(1);

    c->w = w;
    c->locked = false;

    if (list) {
        list->prev->next = c;
        c->prev          = list->prev;
        list->prev       = c;
        c->next          = list;

    } else {
        list = c;
        list->prev = list->next = list;
    }
    tile(w);
    logger("Added window %d", w);
    ws_save(ws);
}

void delwin(Window w) {
    client *x = 0;

    for win if (c->w == w) x = c;

    if (!list || !x)  return;
    if (x->prev == x) list = 0;
    if (list == x)    list = x->next;
    if (x->next)      x->next->prev = x->prev;
    if (x->prev)      x->prev->next = x->next;

    free(x); logger("Deleted window %d", w);
    deltile(w);
    ws_save(ws);
}

void killwin(const Arg arg) {
    if (cur && cur->locked == false) XKillClient(d, cur->w);
    logger("Killed client %d", cur->w);
}

void centerwin(const Arg arg) {
    if(!cur) return;
    if(getwnum(cur->w)) return;

    sizewin(cur->w, &(int){0}, &(int){0}, &ww, &wh);
    XMoveWindow(d, cur->w, (sw - ww) / 2, (sh - wh) / 2); logger("Centered window %d", cur->w);
}

void win_fs(const Arg arg) {
    if (!cur) return;

    if ((cur->f = cur->f ? 0 : 1)) {
        sizewin(cur->w, &cur->wx, &cur->wy, &cur->ww, &cur->wh);
        XMoveResizeWindow(d, cur->w, 0, 0, sw, sh); logger("Wmode -> %d -> Fs", cur->w);
    } else {
        XMoveResizeWindow(d, cur->w, cur->wx, cur->wy, cur->ww, cur->wh); logger("Wmode -> %d !-> Fs", cur->w);
	}
}

void win_to_ws(const Arg arg) {
    int tmp = ws;

    if (arg.i == tmp) return;

    ws_sel(arg.i);
    addwin(cur->w);
    ws_save(arg.i);

    ws_sel(tmp);
    delwin(cur->w);
    XUnmapWindow(d, cur->w);
    ws_save(tmp);

    if (list) focus(list);
    updatetiles();
    logger("Window %d -> %d", cur->w, arg.i);
}

void win_prev(const Arg arg) {
    if (!cur) return;

    XRaiseWindow(d, cur->prev->w);
    focus(cur->prev);
}

void win_next(const Arg arg) {
    if (!cur) return;

    XRaiseWindow(d, cur->next->w);
    focus(cur->next);
}

void ws_go(const Arg arg) {
    int tmp = ws;

    if (arg.i == ws) return;

    ws_save(ws);
    ws_sel(arg.i);

    for win XMapWindow(d, c->w);

    ws_sel(tmp);

    for win XUnmapWindow(d, c->w);

    ws_sel(arg.i);

    if (list) focus(list); else cur = 0;
}

void run(const Arg arg) {
    logger("Running shard..."); if (fork()) return;
    if (d) close(ConnectionNumber(d));
    setsid();
    execvp((char*)arg.com[0], (char**)arg.com);
}

void quit(const Arg arg) {
	if(arg.i) restart = 1;
	running = 0; logger("Exiting...");
}

void grabkeyb(Window root) {
    unsigned int i, j, modifiers[] = {0, LockMask, numlock, numlock|LockMask};
    XModifierKeymap *modmap = XGetModifierMapping(d);
    KeyCode code;

    for (i = 0; i < 8; i++)
        for (int k = 0; k < modmap->max_keypermod; k++)
            if (modmap->modifiermap[i * modmap->max_keypermod + k]
                == XKeysymToKeycode(d, 0xff7f))
                numlock = (1 << i);

    XUngrabKey(d, AnyKey, AnyModifier, root);

    for (i = 0; i < sizeof(keys)/sizeof(*keys); i++)
        if ((code = XKeysymToKeycode(d, keys[i].keysym)))
            for (j = 0; j < sizeof(modifiers)/sizeof(*modifiers); j++)
                XGrabKey(d, code, keys[i].mod | modifiers[j], root,
                        True, GrabModeAsync, GrabModeAsync);

    for (i = 1; i < 4; i += 2)
        for (j = 0; j < sizeof(modifiers)/sizeof(*modifiers); j++)
            XGrabButton(d, i, MOD | modifiers[j], root, True,
                ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
                GrabModeAsync, GrabModeAsync, 0, 0);

    XFreeModifiermap(modmap);
}

void init(void)
{
	XSelectInput(d, root, SubstructureRedirectMask);
	XDefineCursor(d, root, XCreateFontCursor(d, 68)); logger("Created cursor!");
	grabkeyb(root); logger("Grabbed input!");
	setup_hints();
	int stsize = sizeof stcmds / sizeof stcmds[0];
	for(int i=0; i < stsize; i++) 
		system(stcmds[i]);
}

int main(int argc, char* argv[]) {
    XEvent ev;

    if (!(d = XOpenDisplay(0))) die("Couldn't open display"); 
    logger("Initializing shardWM on display %s", XDisplayName(0));
    signal(SIGCHLD, SIG_IGN);
    XSetErrorHandler(xerror);

    int s = DefaultScreen(d);
    root  = RootWindow(d, s);
    sw    = XDisplayWidth(d, s);
    sh    = XDisplayHeight(d, s); 

    init();
    logger("Initialized shardWM");
    while (running && !XNextEvent(d, &ev))
        if (evhandler[ev.type]) evhandler[ev.type](&ev);
    if(restart) execvp(argv[0], argv);
    else { XCloseDisplay(d); logger("Closing display -> %s", XDisplayName(0)); }
    return EXIT_SUCCESS;
}
