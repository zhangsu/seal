PROJDIR       = ../..
SRCDIR        = $(PROJDIR)/src
MPGDIR        = $(PROJDIR)/mpg123
MPGSRCDIR     = $(MPGDIR)/src/
MPGSRCMPGDIR	= $(MPGSRCDIR)/libmpg123
MPGLIBDIR     = $(MPGSRCMPGDIR)/.libs
MSVCDIR				= $(PROJDIR)/msvc

CC            = gcc
LINKER        = gcc
IFLAGS        = -I../../include
FLAGS         = -Wextra -O3
CFLAGS          = $(FLAGS) $(IFLAGS) -DNDEBUG -c
LFLAGS        = $(FLAGS) -shared $(MSVCDIR)/seal/exports.def
LIBS          = ../../msvc/lib/OpenAL32.lib $(MPGLIBDIR)/libmpg123.a
OUTPUT        = seal.dll

OBJECTS       = bitwise.o framing.o bitrate.o block.o codebook.o envelope.o floor0.o floor1.o info.o lookup.o lpc.o lsp.o mapping0.o mdct.o psy.o registry.o res0.o sharedbook.o smallft.o synthesis.o vorbisfile.o window.o buf.o core.o efs.o err.o fmt.o listener.o mpg.o ov.o raw.o reader.o rvb.o src.o stream.o threading.o wav.o
DLLS          = OpenAL32.dll

VPATH         = $(SRCDIR)/libogg $(SRCDIR)/libvorbis $(SRCDIR)/seal

all: $(OUTPUT) $(DLLS)

OpenAL32.dll: $(MSVCDIR)/lib/OpenAL32.dll
	cp $< .

$(OUTPUT): $(OBJECTS) $(LIBS)
	$(LINKER) $(LFLAGS) -o $@ $^

$(MPGLIBDIR)/libmpg123.a: $(MPGSRCDIR)/config.h
	cd $(MPGSRCMPGDIR); make

$(MPGSRCDIR)/config.h:
	cd $(MPGDIR); sh ./configure --disable-shared

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.{o,dll} $(OUTPUT)