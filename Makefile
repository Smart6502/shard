CFLAGS += -std=c99 -Wall -Wextra -pedantic -Wold-style-declaration
CFLAGS += -Wmissing-prototypes -Wno-unused-parameter
PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
CC     ?= gcc
VERSION = 1

all: shard

config.h:
	cp src/config.def.h src/config.h

shard: src/shard.c src/shard.h src/config.h Makefile
	$(CC) -O3 $(CFLAGS) -o build/$@ $< -lX11 $(LDFLAGS)

install: all
	install -Dm755 build/shard $(DESTDIR)$(BINDIR)/shard

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/shard

clean:
	rm -f build/shard build/*.o

.PHONY: all install uninstall clean
