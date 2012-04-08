CC              = gcc
LINKER          = gcc
FLAGS           = -Wextra
CFLAGS          = $(FLAGS) -c -I$(INCLUDEDIR)
LFLAGS          = $(FLAGS)

PROJDIR         = ../..
INCLUDEDIR      = $(PROJDIR)/include
LIBDIR          = $(PROJDIR)/lib
SRCDIR          = $(PROJDIR)/src

OBJDIR          = debug
SEALOBJDIR      = $(OBJDIR)/$(SEAL)
LIBOGGOBJDIR    = $(OBJDIR)/libogg
LIBVORBISOBJDIR = $(OBJDIR)/libvorbis
TESTOBJDIR      = $(OBJDIR)/test

LIBS            = $(addprefix $(LIBDIR)/, OpenAL32.lib libmpg123.lib)
SEALOBJ         = $(addprefix $(SEALOBJDIR)/, buf.o core.o efs.o err.o fmt.o listener.o mpg.o ov.o raw.o reader.o reverb.o src.o stream.o threading.o wav.o)
LIBOGGOBJ       = $(addprefix $(LIBOGGOBJDIR)/, bitwise.o framing.o)
LIBVORBISOBJ    = $(addprefix $(LIBVORBISOBJDIR)/, bitrate.o block.o codebook.o envelope.o floor0.o floor1.o info.o lookup.o lpc.o lsp.o mapping0.o mdct.o psy.o registry.o res0.o sharedbook.o smallft.o synthesis.o vorbisfile.o window.o)
TESTOBJ         = $(addprefix $(TESTOBJDIR)/, test.o test_listener.o test_src.o)

SEAL            = seal
SEALOUTPUT      = $(OBJDIR)/$(SEAL).dll
SEALLIBOUTPUT   = $(OBJDIR)/$(SEAL).lib
TESTOUTPUT      = $(OBJDIR)/test.exe

VPATH           = $(INCLUDEDIR)/$(SEAL):$(SRCDIR)/$(SEAL):$(SRCDIR)/libogg:$(SRCDIR)/libvorbis:$(PROJDIR)/test

COMPILE         = $(CC) $(CFLAGS) -o $@ $<
LINK            = $(LINKER) $(LFLAGS) -o $@ $^
MKDIR           = mkdir -p
CP              = cp $< $@

ifeq ($(RELEASE), 1)
    FLAGS += -O3
    CFLAGS += -DNDEBUG
    OBJDIR = release
endif

.PHONY : clean

all : $(SEALOUTPUT) $(TESTOUTPUT) $(addprefix $(OBJDIR)/, OpenAL32.dll libmpg123.dll wrap_oal.dll)

$(SEALOBJDIR)/ :
	$(MKDIR) $(SEALOBJDIR)/

$(LIBOGGOBJDIR)/ :
	$(MKDIR) $(LIBOGGOBJDIR)/

$(LIBVORBISOBJDIR)/ :
	$(MKDIR) $(LIBVORBISOBJDIR)/

$(TESTOBJDIR)/ :
	$(MKDIR) $(TESTOBJDIR)/

$(OBJDIR)/OpenAL32.dll : $(LIBDIR)/OpenAL32.dll
	$(CP)

$(OBJDIR)/libmpg123.dll : $(LIBDIR)/libmpg123.dll
	$(CP)

$(OBJDIR)/wrap_oal.dll : $(LIBDIR)/wrap_oal.dll
	$(CP)

$(TESTOUTPUT) : $(TESTOBJ) $(SEALOUTPUT)
	$(LINK)

$(SEALOUTPUT) : $(SEALOBJ) $(LIBOGGOBJ) $(LIBVORBISOBJ) $(LIBS) $(PROJDIR)/msvc/$(SEAL)/exports.def
	$(LINK) -shared

$(SEALOBJDIR)/buf.o : buf.c buf.h core.h raw.h fmt.h wav.h stream.h ov.h mpg.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/core.o : core.c core.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/efs.o : efs.c efs.h core.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/err.o : err.c err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/fmt.o : fmt.c fmt.h reader.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/listener.o : listener.c listener.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/mpg.o : mpg.c mpg.h raw.h stream.h fmt.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/ov.o : ov.c ov.h raw.h stream.h fmt.h reader.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/raw.o : raw.c raw.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/reader.o : reader.c reader.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/reverb.o : reverb.c reverb.h core.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/src.o : src.c src.h core.h buf.h raw.h fmt.h stream.h efs.h err.h threading.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/stream.o : stream.c stream.h raw.h fmt.h wav.h ov.h mpg.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/threading.o : threading.c threading.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/wav.o : wav.c wav.h raw.h stream.h fmt.h reader.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(LIBOGGOBJDIR)/%.o : %.c | $(LIBOGGOBJDIR)/
	$(COMPILE)

$(LIBVORBISOBJDIR)/%.o : %.c | $(LIBVORBISOBJDIR)/
	$(COMPILE)

$(TESTOBJDIR)/test.o : test.c test.h | $(TESTOBJDIR)/
	$(COMPILE)

$(TESTOBJDIR)/test_src.o : test_src.c test.h | $(TESTOBJDIR)/
	$(COMPILE)

$(TESTOBJDIR)/test_listener.o : test_listener.c test.h | $(TESTOBJDIR)/
	$(COMPILE)

clean :
	rm -r -f $(OBJDIR)/
