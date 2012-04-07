CC              = gcc
LINKER          = gcc
FLAGS           = -Wextra
CFLAGS          = $(FLAGS) -fPIC -c -I$(INCLUDEDIR)
LFLAGS          = $(FLAGS)

PROJDIR         = ../..
INCLUDEDIR      = $(PROJDIR)/include
SRCDIR          = $(PROJDIR)/src

OBJDIR          = debug
SEALOBJDIR      = $(OBJDIR)/$(SEAL)
LIBOGGOBJDIR    = $(OBJDIR)/libogg
LIBVORBISOBJDIR = $(OBJDIR)/libvorbis
TESTOBJDIR      = $(OBJDIR)/test

LIBS            = -lopenal -lmpg123

SEALOBJ         = $(addprefix $(SEALOBJDIR)/, buf.o core.o efs.o err.o fmt.o listener.o mpg.o ov.o raw.o reader.o reverb.o src.o stream.o threading.o wav.o)
LIBOGGOBJ       = $(addprefix $(LIBOGGOBJDIR)/, bitwise.o framing.o)
LIBVORBISOBJ    = $(addprefix $(LIBVORBISOBJDIR)/, bitrate.o block.o codebook.o envelope.o floor0.o floor1.o info.o lookup.o lpc.o lsp.o mapping0.o mdct.o psy.o registry.o res0.o sharedbook.o smallft.o synthesis.o vorbisfile.o window.o)
TESTOBJ         = $(addprefix $(TESTOBJDIR)/, test.o test_listener.o test_src.o)

SEAL            = seal
SEALOUTPUT      = lib$(SEAL).so
TESTOUTPUT      = test

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

all : $(SEALOUTPUT) $(TESTOUTPUT)

$(SEALOBJDIR)/ :
	$(MKDIR) $(SEALOBJDIR)/

$(LIBOGGOBJDIR)/ :
	$(MKDIR) $(LIBOGGOBJDIR)/

$(LIBVORBISOBJDIR)/ :
	$(MKDIR) $(LIBVORBISOBJDIR)/

$(TESTOBJDIR)/ :
	$(MKDIR) $(TESTOBJDIR)/

$(TESTOUTPUT) : $(TESTOBJ) $(SEALOUTPUT)
	$(LINK) $(LIBS)

$(SEALOUTPUT) : $(SEALOBJ) $(LIBOGGOBJ) $(LIBVORBISOBJ)
	$(LINK) $(LIBS) -shared

$(SEALOBJDIR)/buf.o : buf.c buf.h core.h raw.h fmt.h wav.h stream.h ov.h mpg.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/core.o : core.c core.h threading.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/efs.o : efs.c efs.h core.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/err.o : err.c err.h core.h threading.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/fmt.o : fmt.c fmt.h reader.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/listener.o : listener.c listener.h core.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/mpg.o : mpg.c mpg.h raw.h stream.h fmt.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/ov.o : ov.c ov.h core.h raw.h stream.h fmt.h reader.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/raw.o : raw.c raw.h core.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/reader.o : reader.c reader.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/reverb.o : reverb.c reverb.h core.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/src.o : src.c src.h core.h buf.h raw.h fmt.h stream.h efs.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/stream.o : stream.c stream.h core.h raw.h fmt.h wav.h ov.h mpg.h err.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/threading.o : threading.c threading.h core.h | $(SEALOBJDIR)/
	$(COMPILE)

$(SEALOBJDIR)/wav.o : wav.c wav.h core.h raw.h stream.h fmt.h reader.h err.h | $(SEALOBJDIR)/
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
	rm -f $(SEALOUTPUT) $(TESTOUTPUT)
