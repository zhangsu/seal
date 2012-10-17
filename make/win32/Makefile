PROJDIR       = ../..
SRCDIR        = $(PROJDIR)/src
MPGDIR        = $(PROJDIR)/mpg123
MPGSRCDIR     = $(MPGDIR)/src
MPGSRCMPGDIR  = $(MPGSRCDIR)/libmpg123
MPGLIBDIR     = $(MPGSRCMPGDIR)/.libs
MSVCDIR       = $(PROJDIR)/msvc

CC            = gcc
LINKER        = gcc
IFLAGS        = -I../../include
FLAGS         = -Wextra -O3
CFLAGS        = $(FLAGS) $(IFLAGS) -DNDEBUG -c
LFLAGS        = $(FLAGS) -shared -s $(MSVCDIR)/seal/exports.def
LIBPATH       = -L$(MSVCDIR)/lib -L$(MPGLIBDIR)
LIBS          = -lOpenAL32 -lmpg123
OUTPUT        = seal.dll

OBJECTS       = bitwise.o framing.o bitrate.o block.o codebook.o envelope.o floor0.o floor1.o info.o lookup.o lpc.o lsp.o mapping0.o mdct.o psy.o registry.o res0.o sharedbook.o smallft.o synthesis.o vorbisfile.o window.o buf.o core.o efs.o err.o fmt.o listener.o mpg.o ov.o raw.o reader.o rvb.o src.o stream.o threading.o wav.o

VPATH         = $(SRCDIR)/libogg $(SRCDIR)/libvorbis $(SRCDIR)/seal

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS) $(MPGLIBDIR)/libmpg123.a
	$(LINKER) $(LFLAGS) $(LIBPATH) -o $@ $(OBJECTS) $(LIBS)

$(MPGLIBDIR)/libmpg123.a: $(MPGSRCDIR)/config.h
	cd $(MPGSRCMPGDIR); make

$(MPGSRCDIR)/config.h:
	cd $(MPGDIR); sh ./configure --disable-shared

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o $(OUTPUT)