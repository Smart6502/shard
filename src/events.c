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
void button_press(XEvent *e) {
    if (!e->xbutton.subwindow || getwnum(cur->w)) return;

    sizewin(e->xbutton.subwindow, &wx, &wy, &ww, &wh);
    XRaiseWindow(d, e->xbutton.subwindow);
    mouse = e->xbutton; logger("Event: ButtonPress");
}

void button_release(XEvent *e) {
	if(getwnum(cur->w)) return;
    mouse.subwindow = 0; logger("Event: ButtonRelease");
}

void confreq(XEvent *e) {
    XConfigureRequestEvent *ev = &e->xconfigurerequest;

    XConfigureWindow(d, ev->window, ev->value_mask, &(XWindowChanges) {
        .x          = ev->x,
        .y          = ev->y,
        .width      = ev->width,
        .height     = ev->height,
        .sibling    = ev->above,
        .stack_mode = ev->detail
    }); logger("Event: ConfigureRequest");
}

void mapreq(XEvent *e) {
    Window w = e->xmaprequest.window;

    XSelectInput(d, w, StructureNotifyMask|EnterWindowMask);
    sizewin(w, &wx, &wy, &ww, &wh);
    addwin(w);
    cur = list->prev;

    if (wx + wy == 0 && info.size == 6) centerwin((Arg){0});

    XMapWindow(d, w);
    focus(list->prev);
    logger("Event: MapRequest");
}

void keypress(XEvent *e) {
    KeySym keysym = XkbKeycodeToKeysym(d, e->xkey.keycode, 0, 0);

    for (unsigned int i=0; i < sizeof(keys)/sizeof(*keys); ++i)
        if (keys[i].keysym == keysym &&
            mod_clean(keys[i].mod) == mod_clean(e->xkey.state))
            keys[i].function(keys[i].arg); logger("Event: KeyPress");
}

void notify_enter(XEvent *e) {
    while(XCheckTypedEvent(d, EnterNotify, e));

    for win if (c->w == e->xcrossing.window) focus(c); logger("Event: EnterNotify");
}

void notify_destroy(XEvent *e) {
    delwin(e->xdestroywindow.window);

    if (list) focus(list->prev); logger("Event: DestroyNotify");
}

void notify_mapping(XEvent *e) {
    XMappingEvent *ev = &e->xmapping;

    if (ev->request == MappingKeyboard || ev->request == MappingModifier) {
        XRefreshKeyboardMapping(ev);
        grabkeyb(root);
    }
    logger("Event: MappingNotify");
}

void notify_motion(XEvent *e) {
    if (!mouse.subwindow || cur->f || getwnum(cur->w) || cur->w == info.wins[0]) return;

    while(XCheckTypedEvent(d, MotionNotify, e));

    int xdff = e->xbutton.x_root - mouse.x_root;
    int ydff = e->xbutton.y_root - mouse.y_root;

    XMoveResizeWindow(d, mouse.subwindow,
        wx + (mouse.button == 1 ? xdff : 0),
        wy + (mouse.button == 1 ? ydff : 0),
        MAX(1, ww + (mouse.button == 3 ? xdff : 0)),
        MAX(1, wh + (mouse.button == 3 ? ydff : 0)));
}
