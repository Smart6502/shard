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
} client;

typedef struct wsinfo {
	int size;
	Window wins[15];
	Window master;
} _wsinfo;

void focus(client *c);
void button_press(XEvent *e);
void button_release(XEvent *e);
void confreq(XEvent *e);
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
void win_fs(const Arg arg);
void win_to_ws(const Arg arg);
void win_prev(const Arg arg);
void win_next(const Arg arg);
void ws_go(const Arg arg);
static void run(const Arg arg);
void quit(const Arg arg);
void grabinput(Window root);

static int xerror() { return 0; }
