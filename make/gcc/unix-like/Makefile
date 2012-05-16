CC          = gcc
LINKER      = gcc
CFLAGS      = -Wextra -O3 -I../../../include -DNDEBUG -c -fPIC
LFLAGS      = -Wextra -O3 -shared
LIBS        = -lopenal -lmpg123
COFLAG      = -o 
LOFLAG      = -o 
OUTPUT      = libseal.so

MKDIR       = mkdir -p
CP          = 
RM          = rm -rf
RMDIR       = rmdir

PROJDIR     = ../../..

OBJECTS     = libogg/bitwise.o libogg/framing.o libvorbis/bitrate.o libvorbis/block.o libvorbis/codebook.o libvorbis/envelope.o libvorbis/floor0.o libvorbis/floor1.o libvorbis/info.o libvorbis/lookup.o libvorbis/lpc.o libvorbis/lsp.o libvorbis/mapping0.o libvorbis/mdct.o libvorbis/psy.o libvorbis/registry.o libvorbis/res0.o libvorbis/sharedbook.o libvorbis/smallft.o libvorbis/synthesis.o libvorbis/vorbisfile.o libvorbis/window.o seal/buf.o seal/core.o seal/efs.o seal/err.o seal/fmt.o seal/listener.o seal/mpg.o seal/ov.o seal/raw.o seal/reader.o seal/rvb.o seal/src.o seal/stream.o seal/threading.o seal/wav.o




all : $(OUTPUT) 



$(OUTPUT): $(OBJECTS)
	$(LINKER) $(OBJECTS) $(LIBS) $(LFLAGS) $(LOFLAG)$@
	


libvorbis/timestamp:
	$(MKDIR) libvorbis || echo OK
	echo > libvorbis/timestamp

libvorbis/lookup.o: libvorbis/timestamp ../../../src/libvorbis/lookup.c \
 ../../../src/libvorbis/lookup.h ../../../src/libvorbis/lookup_data.h \
 ../../../src/libvorbis/os.h ../../../include/ogg/os_types.h \
 ../../../src/libvorbis/misc.h ../../../include/vorbis/codec.h \
 ../../../include/ogg/ogg.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/lookup.c $(COFLAG)$@

libvorbis/floor1.o: libvorbis/timestamp ../../../src/libvorbis/floor1.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/floor1.c $(COFLAG)$@

libvorbis/envelope.o: libvorbis/timestamp ../../../src/libvorbis/envelope.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/envelope.c $(COFLAG)$@

libvorbis/info.o: libvorbis/timestamp ../../../src/libvorbis/info.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h ../../../src/libvorbis/window.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/info.c $(COFLAG)$@

libvorbis/floor0.o: libvorbis/timestamp ../../../src/libvorbis/floor0.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h ../../../src/libvorbis/lpc.h \
 ../../../src/libvorbis/lsp.h ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/floor0.c $(COFLAG)$@

libvorbis/codebook.o: libvorbis/timestamp ../../../src/libvorbis/codebook.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codebook.h \
 ../../../src/libvorbis/scales.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/codebook.c $(COFLAG)$@

libvorbis/mapping0.o: libvorbis/timestamp ../../../src/libvorbis/mapping0.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/window.h ../../../src/libvorbis/registry.h \
 ../../../src/libvorbis/lpc.h ../../../src/libvorbis/lsp.h \
 ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/mapping0.c $(COFLAG)$@

libvorbis/block.o: libvorbis/timestamp ../../../src/libvorbis/block.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/window.h ../../../src/libvorbis/lpc.h \
 ../../../src/libvorbis/registry.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/block.c $(COFLAG)$@

libvorbis/registry.o: libvorbis/timestamp ../../../src/libvorbis/registry.c \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../include/ogg/os_types.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/registry.c $(COFLAG)$@

libvorbis/psy.o: libvorbis/timestamp ../../../src/libvorbis/psy.c \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../include/ogg/os_types.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/masking.h ../../../src/libvorbis/lpc.h \
 ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/psy.c $(COFLAG)$@

libvorbis/lsp.o: libvorbis/timestamp ../../../src/libvorbis/lsp.c \
 ../../../src/libvorbis/lsp.h ../../../src/libvorbis/os.h \
 ../../../include/ogg/os_types.h ../../../src/libvorbis/misc.h \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../src/libvorbis/lookup.h ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/lsp.c $(COFLAG)$@

libvorbis/res0.o: libvorbis/timestamp ../../../src/libvorbis/res0.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/res0.c $(COFLAG)$@

libvorbis/synthesis.o: libvorbis/timestamp ../../../src/libvorbis/synthesis.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/synthesis.c $(COFLAG)$@

libvorbis/lpc.o: libvorbis/timestamp ../../../src/libvorbis/lpc.c ../../../src/libvorbis/os.h \
 ../../../include/ogg/os_types.h ../../../src/libvorbis/misc.h \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/lpc.h \
 ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/lpc.c $(COFLAG)$@

libvorbis/bitrate.o: libvorbis/timestamp ../../../src/libvorbis/bitrate.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/bitrate.c $(COFLAG)$@

libvorbis/window.o: libvorbis/timestamp ../../../src/libvorbis/window.c \
 ../../../src/libvorbis/os.h ../../../include/ogg/os_types.h \
 ../../../src/libvorbis/misc.h ../../../include/vorbis/codec.h \
 ../../../include/ogg/ogg.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/window.c $(COFLAG)$@

libvorbis/sharedbook.o: libvorbis/timestamp ../../../src/libvorbis/sharedbook.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../src/libvorbis/os.h ../../../src/libvorbis/misc.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codebook.h \
 ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/sharedbook.c $(COFLAG)$@

libvorbis/mdct.o: libvorbis/timestamp ../../../src/libvorbis/mdct.c \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../include/ogg/os_types.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/os.h ../../../src/libvorbis/misc.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/mdct.c $(COFLAG)$@

libvorbis/vorbisfile.o: libvorbis/timestamp ../../../src/libvorbis/vorbisfile.c \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../include/ogg/os_types.h ../../../include/vorbis/vorbisfile.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/vorbisfile.c $(COFLAG)$@

libvorbis/smallft.o: libvorbis/timestamp ../../../src/libvorbis/smallft.c \
 ../../../src/libvorbis/smallft.h ../../../include/vorbis/codec.h \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../src/libvorbis/os.h ../../../src/libvorbis/misc.h
	$(CC) $(CFLAGS) ../../../src/libvorbis/smallft.c $(COFLAG)$@


libogg/timestamp:
	$(MKDIR) libogg || echo OK
	echo > libogg/timestamp

libogg/bitwise.o: libogg/timestamp ../../../src/libogg/bitwise.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h
	$(CC) $(CFLAGS) ../../../src/libogg/bitwise.c $(COFLAG)$@

libogg/framing.o: libogg/timestamp ../../../src/libogg/framing.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h
	$(CC) $(CFLAGS) ../../../src/libogg/framing.c $(COFLAG)$@


seal/timestamp:
	$(MKDIR) seal || echo OK
	echo > seal/timestamp

seal/efs.o: seal/timestamp ../../../src/seal/efs.c ../../../include/al/al.h \
 ../../../include/al/efx.h ../../../include/seal/efs.h \
 ../../../include/seal/src.h ../../../include/seal/buf.h \
 ../../../include/seal/raw.h ../../../include/seal/err.h \
 ../../../include/seal/fmt.h ../../../include/seal/stream.h \
 ../../../include/seal/core.h ../../../include/seal/err.h
	$(CC) $(CFLAGS) ../../../src/seal/efs.c $(COFLAG)$@

seal/buf.o: seal/timestamp ../../../src/seal/buf.c ../../../include/al/al.h \
 ../../../include/seal/buf.h ../../../include/seal/raw.h \
 ../../../include/seal/err.h ../../../include/seal/fmt.h \
 ../../../include/seal/core.h ../../../include/seal/raw.h \
 ../../../include/seal/fmt.h ../../../include/seal/err.h \
 ../../../src/seal/ov.h ../../../include/seal/stream.h \
 ../../../src/seal/mpg.h ../../../src/seal/wav.h
	$(CC) $(CFLAGS) ../../../src/seal/buf.c $(COFLAG)$@

seal/err.o: seal/timestamp ../../../src/seal/err.c ../../../include/al/al.h \
 ../../../include/seal/err.h
	$(CC) $(CFLAGS) ../../../src/seal/err.c $(COFLAG)$@

seal/core.o: seal/timestamp ../../../src/seal/core.c ../../../include/al/al.h \
 ../../../include/al/alc.h ../../../include/al/efx.h \
 ../../../include/mpg123/mpg123.h ../../../include/seal/core.h \
 ../../../include/seal/err.h ../../../include/seal/err.h
	$(CC) $(CFLAGS) ../../../src/seal/core.c $(COFLAG)$@

seal/fmt.o: seal/timestamp ../../../src/seal/fmt.c ../../../include/seal/fmt.h \
 ../../../include/seal/err.h ../../../include/seal/err.h \
 ../../../src/seal/reader.h
	$(CC) $(CFLAGS) ../../../src/seal/fmt.c $(COFLAG)$@

seal/mpg.o: seal/timestamp ../../../src/seal/mpg.c ../../../include/mpg123/mpg123.h \
 ../../../include/seal/raw.h ../../../include/seal/err.h \
 ../../../include/seal/stream.h ../../../include/seal/raw.h \
 ../../../include/seal/fmt.h ../../../include/seal/err.h \
 ../../../src/seal/mpg.h
	$(CC) $(CFLAGS) ../../../src/seal/mpg.c $(COFLAG)$@

seal/listener.o: seal/timestamp ../../../src/seal/listener.c ../../../include/al/al.h \
 ../../../include/seal/listener.h ../../../include/seal/err.h \
 ../../../include/seal/err.h
	$(CC) $(CFLAGS) ../../../src/seal/listener.c $(COFLAG)$@

seal/ov.o: seal/timestamp ../../../src/seal/ov.c ../../../include/vorbis/codec.h \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/vorbisfile.h ../../../include/vorbis/codec.h \
 ../../../include/seal/raw.h ../../../include/seal/err.h \
 ../../../include/seal/stream.h ../../../include/seal/raw.h \
 ../../../include/seal/fmt.h ../../../include/seal/err.h \
 ../../../src/seal/ov.h ../../../src/seal/reader.h
	$(CC) $(CFLAGS) ../../../src/seal/ov.c $(COFLAG)$@

seal/raw.o: seal/timestamp ../../../src/seal/raw.c ../../../include/seal/raw.h \
 ../../../include/seal/err.h ../../../include/seal/err.h
	$(CC) $(CFLAGS) ../../../src/seal/raw.c $(COFLAG)$@

seal/rvb.o: seal/timestamp ../../../src/seal/rvb.c ../../../include/al/al.h \
 ../../../include/al/efx.h ../../../include/al/efx-presets.h \
 ../../../include/seal/rvb.h ../../../include/seal/err.h \
 ../../../include/seal/core.h ../../../include/seal/err.h
	$(CC) $(CFLAGS) ../../../src/seal/rvb.c $(COFLAG)$@

seal/src.o: seal/timestamp ../../../src/seal/src.c ../../../include/al/al.h \
 ../../../include/al/efx.h ../../../include/seal/src.h \
 ../../../include/seal/buf.h ../../../include/seal/raw.h \
 ../../../include/seal/err.h ../../../include/seal/fmt.h \
 ../../../include/seal/stream.h ../../../include/seal/core.h \
 ../../../include/seal/buf.h ../../../include/seal/stream.h \
 ../../../include/seal/efs.h ../../../include/seal/src.h \
 ../../../include/seal/err.h ../../../src/seal/threading.h
	$(CC) $(CFLAGS) ../../../src/seal/src.c $(COFLAG)$@

seal/wav.o: seal/timestamp ../../../src/seal/wav.c ../../../include/seal/raw.h \
 ../../../include/seal/err.h ../../../include/seal/stream.h \
 ../../../include/seal/raw.h ../../../include/seal/fmt.h \
 ../../../include/seal/err.h ../../../src/seal/reader.h \
 ../../../src/seal/wav.h
	$(CC) $(CFLAGS) ../../../src/seal/wav.c $(COFLAG)$@

seal/stream.o: seal/timestamp ../../../src/seal/stream.c ../../../include/seal/stream.h \
 ../../../include/seal/raw.h ../../../include/seal/err.h \
 ../../../include/seal/fmt.h ../../../include/seal/raw.h \
 ../../../include/seal/fmt.h ../../../include/seal/err.h \
 ../../../src/seal/ov.h ../../../src/seal/mpg.h ../../../src/seal/wav.h
	$(CC) $(CFLAGS) ../../../src/seal/stream.c $(COFLAG)$@

seal/reader.o: seal/timestamp ../../../src/seal/reader.c ../../../include/seal/err.h \
 ../../../src/seal/reader.h
	$(CC) $(CFLAGS) ../../../src/seal/reader.c $(COFLAG)$@

seal/threading.o: seal/timestamp ../../../src/seal/threading.c \
 ../../../src/seal/threading.h
	$(CC) $(CFLAGS) ../../../src/seal/threading.c $(COFLAG)$@



clean:
	$(RM) libvorbis
	$(RMDIR) libvorbis || echo OK

	$(RM) libogg
	$(RMDIR) libogg || echo OK

	$(RM) seal
	$(RMDIR) seal || echo OK

	$(RM) $(OUTPUT)
	$(RM) *.dll *.exp *.lib *.manifest