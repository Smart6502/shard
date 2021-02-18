CFLAGS += -std=c99 -Wall -Wextra -Isrc/ -O2 -pedantic -Wold-style-declaration
CFLAGS += -Wmissing-prototypes -Wno-unused-parameter
PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
CC     ?= gcc
VERSION = 1

all: config shard

config:
	[ -f src/config.h ] || cp src/config.def.h src/config.h

shard: src/shard.c src/config.h Makefile
	[ -d build/ ] || mkdir build
	$(CC) -O3 $(CFLAGS) -o build/$@ $< -lX11 $(LDFLAGS)
	chmod 777 build/

install: all
	install -Dm755 build/shard $(DESTDIR)$(BINDIR)/shard

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/shard

clean:
	rm -rf build/

.PHONY: all install uninstall clean
