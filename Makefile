# todo: make window-stuff not hardcoded and into a variable

TARGET = xmousepasteblock

INSTALL = install
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
RM = rm -f

CC = gcc
CFLAGS += -std=gnu99
CFLAGS += -Wall -Wundef -Wshadow -Wformat-security

LD = $(CC)
LDFLAGS += $(shell pkg-config --libs x11 xi xmu)
LDFLAGS += -lev 

# OpenBSD make:
#CFLAGS  +!= pkg-config --cflags x11 xi
#LDFLAGS +!= pkg-config --libs x11 xi
#CFLAGS  += -I/usr/local/include
#LDFLAGS += -L/usr/local/lib
#$(TARGET): $(TARGET).o
#.SUFFIXES: .o
#.o:
#	$(LD) "$<" $(LDFLAGS) -o "$@"

# OpenBSD gmake:
#CFLAGS  += $(shell pkg-config --cflags x11 xi)
#CFLAGS  += -I/usr/local/include
#LDFLAGS += -L/usr/local/lib

.NOTPARALLEL:

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: clean
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

$(TARGET): $(TARGET).o window-stuff.o
	$(LD) $? $(LDFLAGS) -o "$@"

.PHONY: install
install: $(TARGET)
	$(INSTALL) -Dm 0755 "$(TARGET)" "$(DESTDIR)$(BINDIR)/$(TARGET)"

.PHONY: uninstall
uninstall:
	$(RM) "$(DESTDIR)$(BINDIR)/$(TARGET)"

.PHONY: clean
clean:
	$(RM) $(TARGET) $(TARGET).o window-stuff.o
