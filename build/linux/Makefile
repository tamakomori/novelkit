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
	../../../linguine/build/linux/liblinguine.a \
	../../../mediakit/build/linux/libmediakit.a \
	../../../mediakit/build/linux/libroot/lib/libpng.a \
	../../../mediakit/build/linux/libroot/lib/libjpeg.a \
	../../../mediakit/build/linux/libroot/lib/libwebp.a \
	../../../mediakit/build/linux/libroot/lib/libvorbis.a \
	../../../mediakit/build/linux/libroot/lib/libogg.a \
	../../../mediakit/build/linux/libroot/lib/libbrotlidec.a \
	../../../mediakit/build/linux/libroot/lib/libbrotlicommon.a \
	../../../mediakit/build/linux/libroot/lib/libbz2.a \
	../../../mediakit/build/linux/libroot/lib/libz.a \
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
	objs/novelkit.o \
	objs/scenario.o

all: novelkit

novelkit: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

objs/api.o: ../../src/api.c objs
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

objs/common.o: ../../src/common.c objs
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

objs/novelkit.o: ../../src/novelkit.c
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

objs/scenario.o: ../../src/scenario.c objs
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

objs:
	mkdir -p objs

clean:
	rm -rf objs novelkit
