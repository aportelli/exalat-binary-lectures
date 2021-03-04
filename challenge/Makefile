OS = $(shell uname -s)
CC = /usr/bin/clang

ifeq ($(OS),Darwin)
	DLEXT = dylib
	CRYPTOPREFIX = /usr/local/opt/openssl@1.1/
	CRYPTOFLAGS = -L$(CRYPTOPREFIX)/lib -lcrypto -I$(CRYPTOPREFIX)/include
	CFLAGS = $(CRYPTOFLAGS)
else
	DLEXT = so
	CFLAGS = -fPIC -lcrypto
endif
CM    = crackme.1 crackme.2 crackme.3 crackme.4 crackme.5
TOOLS = bf.3.$(DLEXT) hash.3.$(DLEXT) inject.4.$(DLEXT) aadebug.4.$(DLEXT)

all: $(CM) $(TOOLS)

crackme.3: 3.c 3-opt.c
	$(CC) $(CFLAGS) -c -O3 3-opt.c -o 3-opt.o
	$(CC) $(CFLAGS) -c 3.c -o 3.o
	$(CC) $(CFLAGS) 3-opt.o 3.o -o crackme.3

crackme.%: %.c
	$(CC) $(CFLAGS) -o $@ $<
ifeq ($(OS), Darwin)
	printf '\x07' | dd of=$@ bs=1 seek=160 count=1 conv=notrunc
endif

bf.3.$(DLEXT): 3-bf.c
	$(CC) $(CFLAGS) -g -shared -O3 -o $@ $<

hash.3.$(DLEXT): 3-hash.c
	$(CC) $(CFLAGS) -g -shared -o $@ $<

inject.4.$(DLEXT): 4-inject.c
	$(CC) $(CFLAGS) -g -shared -lbfd -o $@ $<

aadebug.4.$(DLEXT): 4-aadebug.c
	$(CC) $(CFLAGS) -g -shared -lbfd -o $@ $<

clean:
	rm -f $(CM) $(TOOLS)
