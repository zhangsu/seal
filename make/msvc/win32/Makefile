CC          = cl
LINKER      = link
CFLAGS      =  
LFLAGS      =  
LIBS        = ../../../lib/OpenAL32.lib ../../../lib/libmpg123.lib
OFLAG       = /Fo
OUTPUT      = seal.dll

MKDIR       = mkdir
CP          = copy
RM          = del

PROJDIR     = ../../..

OBJECTS     = libogg/bitwise.o libogg/framing.o libvorbis/bitrate.o libvorbis/block.o libvorbis/codebook.o libvorbis/envelope.o libvorbis/floor0.o libvorbis/floor1.o libvorbis/info.o libvorbis/lookup.o libvorbis/lpc.o libvorbis/lsp.o libvorbis/mapping0.o libvorbis/mdct.o libvorbis/psy.o libvorbis/registry.o libvorbis/res0.o libvorbis/sharedbook.o libvorbis/smallft.o libvorbis/synthesis.o libvorbis/vorbisfile.o libvorbis/window.o seal/buf.o seal/core.o seal/efs.o seal/err.o seal/fmt.o seal/listener.o seal/mpg.o seal/ov.o seal/raw.o seal/reader.o seal/reverb.o seal/src.o seal/stream.o seal/threading.o seal/wav.o

all : $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(LINK) $(LFLAGS) $(LIBS)


libogg:
	$(MKDIR) $@

libogg/framing.o: ../../../src/libogg/framing.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libogg/bitwise.o: ../../../src/libogg/bitwise.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<


libvorbis:
	$(MKDIR) $@

libvorbis/bitrate.o: ../../../src/libvorbis/bitrate.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/codebook.o: ../../../src/libvorbis/codebook.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codebook.h \
 ../../../src/libvorbis/scales.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/envelope.o: ../../../src/libvorbis/envelope.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/floor0.o: ../../../src/libvorbis/floor0.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h ../../../src/libvorbis/lpc.h \
 ../../../src/libvorbis/lsp.h ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/block.o: ../../../src/libvorbis/block.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/window.h ../../../src/libvorbis/lpc.h \
 ../../../src/libvorbis/registry.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/lookup.o: ../../../src/libvorbis/lookup.c \
 ../../../src/libvorbis/lookup.h ../../../src/libvorbis/lookup_data.h \
 ../../../src/libvorbis/os.h ../../../include/ogg/os_types.h \
 ../../../src/libvorbis/misc.h ../../../include/vorbis/codec.h \
 ../../../include/ogg/ogg.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/info.o: ../../../src/libvorbis/info.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h ../../../src/libvorbis/window.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/lpc.o: ../../../src/libvorbis/lpc.c ../../../src/libvorbis/os.h \
 ../../../include/ogg/os_types.h ../../../src/libvorbis/misc.h \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/lpc.h \
 ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/floor1.o: ../../../src/libvorbis/floor1.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/lsp.o: ../../../src/libvorbis/lsp.c \
 ../../../src/libvorbis/lsp.h ../../../src/libvorbis/os.h \
 ../../../include/ogg/os_types.h ../../../src/libvorbis/misc.h \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../src/libvorbis/lookup.h ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/registry.o: ../../../src/libvorbis/registry.c \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../include/ogg/os_types.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/mapping0.o: ../../../src/libvorbis/mapping0.c \
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
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/mdct.o: ../../../src/libvorbis/mdct.c \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../include/ogg/os_types.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/os.h ../../../src/libvorbis/misc.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/sharedbook.o: ../../../src/libvorbis/sharedbook.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../src/libvorbis/os.h ../../../src/libvorbis/misc.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codebook.h \
 ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/res0.o: ../../../src/libvorbis/res0.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/synthesis.o: ../../../src/libvorbis/synthesis.c \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/registry.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/psy.o: ../../../src/libvorbis/psy.c \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../include/ogg/os_types.h ../../../src/libvorbis/codec_internal.h \
 ../../../src/libvorbis/envelope.h ../../../src/libvorbis/mdct.h \
 ../../../src/libvorbis/codebook.h ../../../src/libvorbis/psy.h \
 ../../../src/libvorbis/smallft.h ../../../src/libvorbis/backends.h \
 ../../../src/libvorbis/bitrate.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h ../../../src/libvorbis/highlevel.h \
 ../../../src/libvorbis/masking.h ../../../src/libvorbis/lpc.h \
 ../../../src/libvorbis/scales.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/smallft.o: ../../../src/libvorbis/smallft.c \
 ../../../src/libvorbis/smallft.h ../../../include/vorbis/codec.h \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../src/libvorbis/os.h ../../../src/libvorbis/misc.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/vorbisfile.o: ../../../src/libvorbis/vorbisfile.c \
 ../../../include/vorbis/codec.h ../../../include/ogg/ogg.h \
 ../../../include/ogg/os_types.h ../../../include/vorbis/vorbisfile.h \
 ../../../include/vorbis/codec.h ../../../src/libvorbis/os.h \
 ../../../src/libvorbis/misc.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

libvorbis/window.o: ../../../src/libvorbis/window.c \
 ../../../src/libvorbis/os.h ../../../include/ogg/os_types.h \
 ../../../src/libvorbis/misc.h ../../../include/vorbis/codec.h \
 ../../../include/ogg/ogg.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<


seal:
	$(MKDIR) $@

seal/buf.o: ../../../src/seal/buf.c ../../../include/al/al.h \
 ../../../include/seal/buf.h ../../../include/seal/raw.h \
 ../../../include/seal/err.h ../../../include/seal/fmt.h \
 ../../../include/seal/core.h ../../../include/seal/raw.h \
 ../../../include/seal/fmt.h ../../../include/seal/err.h \
 ../../../src/seal/ov.h ../../../include/seal/stream.h \
 ../../../src/seal/mpg.h ../../../src/seal/wav.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/efs.o: ../../../src/seal/efs.c ../../../include/al/al.h \
 ../../../include/al/efx.h ../../../include/seal/efs.h \
 ../../../include/seal/src.h ../../../include/seal/buf.h \
 ../../../include/seal/raw.h ../../../include/seal/err.h \
 ../../../include/seal/fmt.h ../../../include/seal/stream.h \
 ../../../include/seal/core.h ../../../include/seal/err.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/core.o: ../../../src/seal/core.c ../../../include/al/al.h \
 ../../../include/al/alc.h ../../../include/al/efx.h \
 ../../../include/mpg123/mpg123.h ../../../include/seal/core.h \
 ../../../include/seal/err.h ../../../include/seal/err.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/err.o: ../../../src/seal/err.c ../../../include/al/al.h \
 ../../../include/seal/err.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/fmt.o: ../../../src/seal/fmt.c ../../../include/seal/fmt.h \
 ../../../include/seal/err.h ../../../include/seal/err.h \
 ../../../src/seal/reader.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/listener.o: ../../../src/seal/listener.c ../../../include/al/al.h \
 ../../../include/seal/listener.h ../../../include/seal/err.h \
 ../../../include/seal/err.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/raw.o: ../../../src/seal/raw.c ../../../include/seal/raw.h \
 ../../../include/seal/err.h ../../../include/seal/err.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/ov.o: ../../../src/seal/ov.c ../../../include/vorbis/codec.h \
 ../../../include/ogg/ogg.h ../../../include/ogg/os_types.h \
 ../../../include/vorbis/vorbisfile.h ../../../include/vorbis/codec.h \
 ../../../include/seal/raw.h ../../../include/seal/err.h \
 ../../../include/seal/stream.h ../../../include/seal/raw.h \
 ../../../include/seal/fmt.h ../../../include/seal/err.h \
 ../../../src/seal/ov.h ../../../src/seal/reader.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/reverb.o: ../../../src/seal/reverb.c ../../../include/al/al.h \
 ../../../include/al/efx.h ../../../include/seal/reverb.h \
 ../../../include/seal/err.h ../../../include/seal/core.h \
 ../../../include/seal/err.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/mpg.o: ../../../src/seal/mpg.c ../../../include/mpg123/mpg123.h \
 ../../../include/seal/raw.h ../../../include/seal/err.h \
 ../../../include/seal/stream.h ../../../include/seal/raw.h \
 ../../../include/seal/fmt.h ../../../include/seal/err.h \
 ../../../src/seal/mpg.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/stream.o: ../../../src/seal/stream.c ../../../include/seal/stream.h \
 ../../../include/seal/raw.h ../../../include/seal/err.h \
 ../../../include/seal/fmt.h ../../../include/seal/raw.h \
 ../../../include/seal/fmt.h ../../../include/seal/err.h \
 ../../../src/seal/ov.h ../../../src/seal/mpg.h ../../../src/seal/wav.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/reader.o: ../../../src/seal/reader.c ../../../include/seal/err.h \
 ../../../src/seal/reader.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/src.o: ../../../src/seal/src.c ../../../include/al/al.h \
 ../../../include/al/efx.h ../../../include/seal/src.h \
 ../../../include/seal/buf.h ../../../include/seal/raw.h \
 ../../../include/seal/err.h ../../../include/seal/fmt.h \
 ../../../include/seal/stream.h ../../../include/seal/core.h \
 ../../../include/seal/buf.h ../../../include/seal/stream.h \
 ../../../include/seal/efs.h ../../../include/seal/src.h \
 ../../../include/seal/err.h ../../../src/seal/threading.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/wav.o: ../../../src/seal/wav.c ../../../include/seal/raw.h \
 ../../../include/seal/err.h ../../../include/seal/stream.h \
 ../../../include/seal/raw.h ../../../include/seal/fmt.h \
 ../../../include/seal/err.h ../../../src/seal/reader.h \
 ../../../src/seal/wav.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<

seal/threading.o: ../../../src/seal/threading.c \
 ../../../src/seal/threading.h
	$(CC) $(CFLAGS) $(OFLAG)$@ $<



clean:
	$(RM) $(SEALOBJDIR)
	$(RM) $(LIBOGGOBJDIR)
	$(RM) $(LIBVORBISOBJDIR)
	$(RM) $(SEALOUTPUT)
	$(RM) *.dll