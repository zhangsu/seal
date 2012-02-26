Scorched End Audio Library
==========================

SEAL is a C library with Ruby binding for 3D audio rendering and manipulation
with built-in support for Ogg Vorbis, MPEG Audio and WAVE format. The major
components of SEAL are based on OpenAL, a 3D sound rendering engine.

### Supported Platforms

Windows and Linux are supported. The third-party libraries are redistributed
in binary forms for 32-bit Windows only; Linux users are expected to install
the third-party libraries themselves. The Makefiles are written specifically
for MSVC and GCC (MinGW or native Linux). In theory seal is portable to all
UN*X-like systems, but it is only tested on Linux.

### Build

-   GCC + GNU Make + Linux

    install [libopenal]
    (http://connect.creativelabs.com/openal/Downloads/Forms/AllItems.aspx)
    and [libmpg123](http://sourceforge.net/projects/mpg123/), then

        $ cd gmake/linux
        $ make RELEASE=1

-   GCC + MinGW

        $ cd gmake/win32
        $ make RELEASE=1

-   MSVC + nmake

        > cd msvc/nmake
        > nmake

-   MSVC + Microsoft Visual Studio 2010

    use the solution and project files under /msvc.

-   Ruby extension

        $ cd demo
        $ ruby configure.rb
        $ make
        
    The default output is `seal.{so,dll}`, which is a dynamic library that
    could be required by Ruby at runtime.

Note that these Makefiles do not add header dependencies for third-party
libraries and other header files which are not expected to change.

### Thread Safety

SEAL can be safely used in a multi-threaded environment so long as no SEAL
source, buffer or stream instance is accessed by different threads at the
*same time* , which is similar to libvorbis' thread-safety conditions
described [here](http://xiph.org/vorbis/doc/vorbisfile/threads.html).

Exceptions are the two functions `seal_startup` and `seal_cleanup`, which are
*NOT* thread-safe. Refer to the documentation for these two functions for
details.

### Character Encoding

UTF-8 should be used to encode the source code or at least the path strings so
that SEAL can properly input audio files using paths that contain multibyte
(Chinese, Japanese, etc.) characters.

### Redistribution

See COPYING.

### Authors

See AUTHOR.

### Etymology

The phrase "Scorched end" (Chinese: 焦尾; Pinyin: Jiao Wei) is a direct
tranlation of the name of a Guqin[1] existed in China in the second
century CE. The name literally means that one end of the Qin is scorched.

This Qin was found by Cai Yong.[2] According to the "History of the Later
Han",[3] Cai Yong once heard very loud sound of someone burning Firmiana
simplex[4] wood for cooking and realized that it is a good material for
making Qin soundboards. He then asked for the wood and cut it into a Qin,
and the sound it produced was pleasant to hear as expected. However, one
end of the wood had already been scorched by the time Cai Yong got it,
hence the name "Scorched end".

SEAL was named so with the hope that it will be more useful than it seems
and will be discovered by the right people who need the right tool.

[1] http://en.wikipedia.org/wiki/Guqin  
[2] http://en.wikipedia.org/wiki/Cai_Yong  
[3] http://en.wikipedia.org/wiki/History_of_the_Later_Han  
[4] http://en.wikipedia.org/wiki/Firmiana_simplex  
