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
void tile(const Window w);
int getwnum(const Window w);
void updatetiles(void);
void deltile(const Window w);

// # MESSIEST CODE IN THE WHOLE PROJECT :)

void updatetiles(void) {
	if(info.master && info.size == 1) {
		logger("Updating primary master...");
		XResizeWindow(d, info.master, sw, sh);
		XMoveWindow(d, info.master, 0, 0);
	} else if(info.size == 2) {
		logger("Updating master...");
		XResizeWindow(d, info.master, (sw*tile_perc)/100, sh);
		XResizeWindow(d, info.wins[1], (sw*(100 - tile_perc))/100, sh);
		XMoveWindow(d, info.wins[1], (sw*tile_perc)/100, 0);
	} else {
		XResizeWindow(d, info.master, (sw*tile_perc)/100, sh);
		XMoveWindow(d, info.master, 0, 0);
		logger("Updating stack....");
		for(int i=0; i < info.size; i++)
			logger("Tiled: %d -> %d", i, info.wins[i]);
		int stkh = sh/(info.size - 1);
		int stkw = (sw*(100 - tile_perc))/100;
		int inch = sh/(info.size - 1);
		for (int i=1; i < info.size; i++) {
			if(i == 1) {
				XResizeWindow(d, info.wins[1], stkw, stkh);
				XMoveWindow(d, info.wins[1], (sw*tile_perc)/100, 0);
			} else {
				XResizeWindow(d, info.wins[i], stkw, stkh);
				XMoveWindow(d, info.wins[i], (sw*tile_perc)/100, inch);
				inch = inch + stkh;
			}
		}
	}
}

void tile(const Window w) {
	if (!info.master) {
		info.wins[0] = w;
		info.master = w;
		info.size++;
		logger("Set up master %d", info.wins[0]);

	} else if (info.size == 1) {		
		info.wins[1] = w;
		info.size++;

	} else {
		info.wins[info.size] = w;
		info.size++;
	}
	updatetiles();
}

// Get window index
int getwnum(const Window w) {
	int wnum=0;
	for (int i=0; i < info.size; i++) {
		if(info.wins[i] == w) wnum = i;
	}
	return wnum;
}


// FIX THIS NOT WORKING PROPERLY !!
void deltile(const Window w) {
	if(info.size == 0) return;
	int wi = getwnum(w);
	if(wi == 0) {
		if(info.size == 1) {
			info.wins[0] = info.wins[1];
			info.master = !info.master;
			logger("Master deleted %d", info.master);
			info.size = 0;
		} else {
			for(int i=0; i < info.size; i++)
				info.wins[i] = info.wins[i+1];
			info.master = info.wins[0];
			info.wins[info.size] = !info.wins[info.size];
			info.size--;
		}
	} else {
		int k;
		for(k=getwnum(w); k < (info.size - 1); k++)
			info.wins[k] = info.wins[k+1];
		info.wins[info.size] = 0;
		info.size--;
	}
	updatetiles();
}


