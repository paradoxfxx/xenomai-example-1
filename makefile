XENO_CONFIG=/usr/xenomai/bin/xeno-config
CC = g++
#CC = $(shell $(XENO_CONFIG) --$(SKIN) --cflags)
SKIN = native
CFLAGS = $(shell $(XENO_CONFIG) --$(SKIN) --cflags)
LDFLAGS = $(shell $(XENO_CONFIG) --$(SKIN) --ldflags)

EXECUTABLE = test

$(EXECUTABLE): main.cpp
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

