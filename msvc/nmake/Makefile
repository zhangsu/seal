CC              = cl
LINKER          = link
CFLAGS          = /c /Ob2ity /Gd /GL /Gy /MD /DNDEBUG /I ..\..\include /TC /nologo
LFLAGS          = /LTCG /OPT:REF /OPT:ICF=3 /MANIFEST /NOLOGO
HEADERDIR       = ../../include/seal
SEALSRCDIR      = ../../src/seal
LIBOGGSRCDIR    = ../../src/libogg
LIBVORBISSRCDIR = ../../src/libvorbis
LIBDIR          = ../../lib
TESTSRCDIR      = ../../test
LIBS            = ../../lib/OpenAL32.lib ../../lib/libmpg123.lib
SEALOBJ         = buf.obj core.obj err.obj fmt.obj listener.obj mid.obj mpg.obj ov.obj raw.obj reader.obj src.obj stream.obj threading.obj wav.obj
LIBOGGOBJ       = bitwise.obj framing.obj
LIBVORBISOBJ    = bitrate.obj block.obj codebook.obj envelope.obj floor0.obj floor1.obj info.obj lookup.obj lpc.obj lsp.obj mapping0.obj mdct.obj psy.obj registry.obj res0.obj sharedbook.obj smallft.obj synthesis.obj vorbisfile.obj window.obj
TESTOBJ         = test.obj test_listener.obj test_src.obj
MODULEDEF       = ../seal/exports.def
SEALOUTPUT      = seal.dll
SEALLIBOUTPUT   = seal.lib
TESTOUTPUT      = test.exe

all : $(SEALOUTPUT) $(TESTOUTPUT) OpenAL32.dll libmpg123.dll wrap_oal.dll

OpenAL32.dll : $(LIBDIR)/$@
	copy /Y ..\..\lib\$@ .

libmpg123.dll : $(LIBDIR)/$@
	copy /Y ..\..\lib\$@ .

wrap_oal.dll : $(LIBDIR)/$@
	copy /Y ..\..\lib\$@ .

$(TESTOUTPUT) : $(TESTOBJ) $(SEALLIBOUTPUT)
	$(LINKER) /OUT:$@ $(LFLAGS) /SUBSYSTEM:CONSOLE $(TESTOBJ) $(SEALLIBOUTPUT)
	mt -nologo -manifest $@.manifest -outputresource:$@;1

$(SEALOUTPUT) : $(SEALOBJ) $(LIBOGGOBJ) $(LIBVORBISOBJ)
	$(LINKER) /OUT:$@ /DLL $(LFLAGS) $(SEALOBJ) $(LIBOGGOBJ) $(LIBVORBISOBJ) $(LIBDIR)/OpenAL32.lib $(LIBDIR)/libmpg123.lib /DEF:$(MODULEDEF)
	mt -nologo -manifest $@.manifest -outputresource:$@;2

buf.obj : $(SEALSRCDIR)/buf.c $(HEADERDIR)/buf.h $(HEADERDIR)/core.h $(HEADERDIR)/raw.h $(HEADERDIR)/fmt.h $(HEADERDIR)/wav.h $(HEADERDIR)/ov.h $(HEADERDIR)/mpg.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/buf.c

core.obj : $(SEALSRCDIR)/core.c $(HEADERDIR)/core.h $(HEADERDIR)/threading.h $(HEADERDIR)/mid.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/core.c

err.obj : $(SEALSRCDIR)/err.c $(HEADERDIR)/err.h $(HEADERDIR)/core.h $(HEADERDIR)/threading.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/err.c

fmt.obj : $(SEALSRCDIR)/fmt.c $(HEADERDIR)/fmt.h $(HEADERDIR)/reader.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/fmt.c

listener.obj : $(SEALSRCDIR)/listener.c $(HEADERDIR)/listener.h $(HEADERDIR)/core.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/listener.c

mid.obj : $(SEALSRCDIR)/mid.c $(HEADERDIR)/mid.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/mid.c

mpg.obj : $(SEALSRCDIR)/mpg.c $(HEADERDIR)/mpg.h $(HEADERDIR)/raw.h $(HEADERDIR)/stream.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/mpg.c

ov.obj : $(SEALSRCDIR)/ov.c $(HEADERDIR)/ov.h $(HEADERDIR)/core.h $(HEADERDIR)/raw.h $(HEADERDIR)/stream.h $(HEADERDIR)/reader.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/ov.c

raw.obj : $(SEALSRCDIR)/raw.c $(HEADERDIR)/raw.h $(HEADERDIR)/core.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/raw.c

reader.obj : $(SEALSRCDIR)/reader.c $(HEADERDIR)/reader.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/reader.c

src.obj : $(SEALSRCDIR)/src.c $(HEADERDIR)/src.h $(HEADERDIR)/core.h $(HEADERDIR)/buf.h $(HEADERDIR)/stream.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/src.c

stream.obj : $(SEALSRCDIR)/stream.c $(HEADERDIR)/stream.h $(HEADERDIR)/core.h $(HEADERDIR)/raw.h $(HEADERDIR)/fmt.h $(HEADERDIR)/wav.h $(HEADERDIR)/ov.h $(HEADERDIR)/mpg.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/stream.c

threading.obj : $(SEALSRCDIR)/threading.c $(HEADERDIR)/threading.h $(HEADERDIR)/core.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/threading.c

wav.obj : $(SEALSRCDIR)/wav.c $(HEADERDIR)/wav.h $(HEADERDIR)/core.h $(HEADERDIR)/raw.h $(HEADERDIR)/stream.h $(HEADERDIR)/reader.h $(HEADERDIR)/err.h
	$(CC) $(CFLAGS) $(SEALSRCDIR)/wav.c

bitwise.obj : $(LIBOGGSRCDIR)/bitwise.c
	$(CC) $(CFLAGS) $(LIBOGGSRCDIR)/bitwise.c

framing.obj : $(LIBOGGSRCDIR)/framing.c
	$(CC) $(CFLAGS) $(LIBOGGSRCDIR)/framing.c

bitrate.obj : $(LIBVORBISSRCDIR)/bitrate.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/bitrate.c

block.obj : $(LIBVORBISSRCDIR)/block.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/block.c

codebook.obj : $(LIBVORBISSRCDIR)/codebook.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/codebook.c

envelope.obj : $(LIBVORBISSRCDIR)/envelope.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/envelope.c

floor0.obj : $(LIBVORBISSRCDIR)/floor0.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/floor0.c

floor1.obj : $(LIBVORBISSRCDIR)/floor1.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/floor1.c

info.obj : $(LIBVORBISSRCDIR)/info.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/info.c

lookup.obj : $(LIBVORBISSRCDIR)/lookup.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/lookup.c

lpc.obj : $(LIBVORBISSRCDIR)/lpc.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/lpc.c

lsp.obj : $(LIBVORBISSRCDIR)/lsp.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/lsp.c

mapping0.obj : $(LIBVORBISSRCDIR)/mapping0.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/mapping0.c

mdct.obj : $(LIBVORBISSRCDIR)/mdct.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/mdct.c

psy.obj : $(LIBVORBISSRCDIR)/psy.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/psy.c

registry.obj : $(LIBVORBISSRCDIR)/registry.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/registry.c

res0.obj : $(LIBVORBISSRCDIR)/res0.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/res0.c

sharedbook.obj : $(LIBVORBISSRCDIR)/sharedbook.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/sharedbook.c

smallft.obj : $(LIBVORBISSRCDIR)/smallft.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/smallft.c

synthesis.obj : $(LIBVORBISSRCDIR)/synthesis.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/synthesis.c

vorbisfile.obj : $(LIBVORBISSRCDIR)/vorbisfile.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/vorbisfile.c

window.obj : $(LIBVORBISSRCDIR)/window.c
	$(CC) $(CFLAGS) $(LIBVORBISSRCDIR)/window.c

test.obj : $(TESTSRCDIR)/test.c $(TESTSRCDIR)/test.h
	$(CC) $(CFLAGS) $(TESTSRCDIR)/test.c

test_listener.obj : $(TESTSRCDIR)/test_listener.c $(TESTSRCDIR)/test.h
	$(CC) $(CFLAGS) $(TESTSRCDIR)/test_listener.c

test_src.obj : $(TESTSRCDIR)/test_src.c $(TESTSRCDIR)/test.h
	$(CC) $(CFLAGS) $(TESTSRCDIR)/test_src.c
    
clean : clean_intermediate
	@if exist *.dll del *.dll
	@if exist *.so  del *.so
	@if exist *.lib del *.lib
	@if exist *.exe del *.exe

clean_intermediate:
	@if exist *.obj del *.obj
	@if exist *.exp del *.exp
    @if exist *.manifest del *.manifest
