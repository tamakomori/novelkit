CC=gcc
LD=ld
AR=ar
STRIP=strip

CPPFLAGS=\
	-I../../../linguine/include \
	-I../../../mediakit/include

CFLAGS=\
	-O0 \
	-g3 \
	-ffast-math \
	-ftree-vectorize \
	-std=gnu11 \
	-Wall \
	-Werror \
	-Wextra \
	-Wundef \
	-Wconversion \
	-Wno-multichar

LDFLAGS=\
	../../../linguine/build/linux-static/liblinguine.a \
	../../../mediakit/build/linux-static/libmediakit.a \
	-lX11 \
	-lasound \
	-lasound \
	-lX11 \
	-lXpm \
	-lGL \
	-lGLX \
	-lpthread \
	-lm

OBJS=\
	objs/api.o \
	objs/common.o \
	objs/main.o \
	objs/scenario.o

all: novelkit

novelkit: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

objs/api.o: ../../src/api.c objs
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

objs/common.o: ../../src/common.c objs
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

objs/main.o: ../../src/main.c
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

objs/scenario.o: ../../src/scenario.c objs
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

objs:
	mkdir -p objs

clean:
	rm -rf objs novelkit
