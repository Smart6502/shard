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
#ifndef CONFIG_H
#define CONFIG_H

#define MOD Mod4Mask

const char* menu[]    	 = {"dmenu_run -sb '#5e81ac'",		   	0};
const char* term[]    	 = {"termite",				    	0};
const char* screenshot[] = {"flameshot gui",	            	    	0};
const char* lock[]	 = {"sh /home/xenon/.local/bin/lock",	    	0};
const char* briup[]   	 = {"brightnessctl", "set", "+5%", 	   	0};
const char* bridown[] 	 = {"brightnessctl", "set", "5%-",	  	0};
const char* voldown[] 	 = {"amixer", "sset", "Master", "5%-",		0};
const char* volup[]   	 = {"amixer", "sset", "Master", "5%+",		0};
const char* volmute[] 	 = {"amixer", "sset", "Master", "toggle",	0};

static struct key keys[] = {
    {MOD,      XK_q,   killwin,   {0}},
    {MOD,      XK_c,   centerwin, {0}},
    {MOD,      XK_f,   win_fs,     {0}},

    {Mod1Mask,           XK_Tab, win_next,   {0}},
    {Mod1Mask|ShiftMask, XK_Tab, win_prev,   {0}},

    {MOD, XK_d,		run, {.com = menu}},
    {MOD, XK_p,		run, {.com = screenshot}},
    {MOD, XK_x,		run, {.com = term}},
    {MOD, XK_i,	     	run, {.com = lock}},

    {0,   XF86XK_AudioLowerVolume,  run, {.com = voldown}},
    {0,   XF86XK_AudioRaiseVolume,  run, {.com = volup}},
    {0,   XF86XK_AudioMute,         run, {.com = volmute}},
    {0,   XF86XK_MonBrightnessUp,   run, {.com = briup}},
    {0,   XF86XK_MonBrightnessDown, run, {.com = bridown}},

    {MOD,           XK_1, ws_go,     {.i = 1}},
    {MOD|ShiftMask, XK_1, win_to_ws, {.i = 1}},
    {MOD,           XK_2, ws_go,     {.i = 2}},
    {MOD|ShiftMask, XK_2, win_to_ws, {.i = 2}},
    {MOD,           XK_3, ws_go,     {.i = 3}},
    {MOD|ShiftMask, XK_3, win_to_ws, {.i = 3}},
    {MOD,           XK_4, ws_go,     {.i = 4}},
    {MOD|ShiftMask, XK_4, win_to_ws, {.i = 4}},
    {MOD,           XK_5, ws_go,     {.i = 5}},
    {MOD|ShiftMask, XK_5, win_to_ws, {.i = 5}},
    {MOD,           XK_6, ws_go,     {.i = 6}},
    {MOD|ShiftMask, XK_6, win_to_ws, {.i = 6}},

    {MOD|ControlMask, XK_q, quit,	{.i = 0}},
    {MOD|ControlMask, XK_r, quit,	{.i = 1}},
};

#endif
