# Scorched End Audio Library

Seal is a C library with Ruby binding for audio rendering and manipulation,
such as direction and distance attenuation, the simulation of the Doppler
effect and reverberation, in a 3D space. It is built on top of [OpenAL]
(http://connect.creativelabs.com/openal/default.aspx), adding support for
audio streaming and audio formats like Ogg Vorbis, MPEG Audio and WAVE.

## Basic Use

Initialize Seal:

```ruby
Seal.startup

include Seal
```

Use a source object to represent a sound source, and attach an audio buffer:

```ruby
source = Source.new
source.buffer = Buffer.new("audio.ogg")
```

Change the position of the source:

```ruby
source.position = 3, 2, -4
```

Change the position of the listener (a singleton of Seal::Listener):

```ruby
Seal.listener.position = -1, -1, 0
```

Play the source:

```ruby
source.play
```

In case of massive audio resource, use of buffer will eat all the memory, so
we can use streams:

```ruby
source.stream = Stream.open("background_music.ogg")
```

Make sure to detach the audio from the source before switching from a buffer
to a stream or vice-versa:

```ruby
source.buffer = ...

# ...

source.detach
source.stream = ...
```

Apply a reverberation effect to the sound source:

```ruby
# Allocate an effect slot and associate a specific reverb object.
slot = EffectSlot.new(Reverb.new(Reverb::Preset::FOREST))
# Start feeding the slot.
slot.feed(0, source)
```

You can find detailed documentations in the header files under `include/seal`
for each of the modules.

The C interface is very similar to the Ruby binding, except that some of the
Seal objects are abbreviated (but still more verbose than Ruby in general):

src -> Source  
buf -> Buffer  
rvb -> Reverb  
efs -> EffectSlot

For example:

```c
seal_src_t src;
seal_buf_t buf;

seal_startup(0);

seal_init_src(&src);
seal_init_buf(&buf);
seal_load2buf(&buf, "audio.ogg", SEAL_UNKNOWN_FMT);
seal_set_src_buf(&src, &buf);
seal_play_src(&src);

// Wait to hear.
_seal_sleep(3000);

seal_destroy_src(&src);
seal_destroy_buf(&buf);
seal_destroy_efs(&efs);
seal_destroy_rvb(&rvb);
seal_cleanup();
```

## Platforms

Linux, Windows and Mac OS X are officially tested and supported. Seal should
run on all Unix-like operating systems where OpenAL, libogg, libvorbis and
libmpg123 can run, but those platforms are never tested. The Makefiles are
generated specifically for MSVC and GCC (MinGW or native Unix-like systems).

## Build

Seal has native dependencies.

### GCC + GNU Make + Unix-like operating systems + UN*X shell

First, you need [CMake](http://www.cmake.org/) 2.4 or later to install
[libopenal](http://kcat.strangesoft.net/openal.html) as follows:

```bash
git clone git://repo.or.cz/openal-soft.git openal-soft
cd openal-soft/build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
make install
```

Then download [libmpg123](http://sourceforge.net/projects/mpg123/files)
and:

```bash
cd <libmpg123_directory>
./configure
cd src/libmpg123
make
make install
```

Finally from Seal directory:

```bash
cd make/gcc/unix-like
make
```

Note: There has been issues compiling OpenAL on some versions of OS X
because LLVM is the default compiler. I haven't had luck compiling
OpenAL with LLVM, so I explicitly specified GCC instead:

```bash
...
CC=/usr/bin/gcc-4.2 cmake -DCMAKE_BUILD_TYPE=Release ..
...
```

### GCC + GNU Make + MinGW + UN*X shell

```bash
cd make/gcc/win32
make
```

### MSVC + nmake + Command Prompt

```bash
cd make/msvc/win32
nmake
```

### MSVC + Microsoft Visual Studio 2010

use the solution and project files under /msvc.

### Ruby extension

```bash
bundle install
rake compile
```

The default output is `lib/seal.{so,dll,bundle}`, which is a dynamic library
that could be required by Ruby at runtime.

Note that Win32 binaries are shipped with the project for compiling Seal on
Windows. You can of course compile your own copies of the dependency
libraries on Windows, but I feel like Win32 users do not usually do that.

## Demos

Use `rake demo:<demo_name>` to run the demos under the `demo/` directory.

-   playback

    Basic playback of audio.

-   control

    Various controls of audio like play, pause, stop.

-   pitch

    Audio pitch shifting.

-   walk

    3D audio positioning (attenuation).

-   reverb

    Audio reverberation. There are lots of built-in reverb presets, but this
    demo only simulates a large room in ice palace.

## Thread Safety

Seal can be safely used in a multi-threaded environment so long as no Seal
source, buffer or stream instance is accessed by different threads at the
*same time* , which is similar to libvorbis' thread-safety conditions
described [here](http://xiph.org/vorbis/doc/vorbisfile/threads.html).

Exceptions are the two functions `seal_startup` and `seal_cleanup`, which are
*NOT* thread-safe. Refer to the documentation for these two functions for
details.

## Character Encoding

UTF-8 should be used to encode the source code or at least the path strings so
that Seal can properly input audio files using paths that contain multi-byte
(Chinese, Japanese, etc.) characters.

## Redistribution

See COPYING.

## Authors

See AUTHOR.

## Etymology

The phrase "Scorched end" (Chinese: 焦尾; Pinyin: Jiao Wei) is a direct
translation of the name of a Guqin[1] existed in China in the second
century CE. The name literally means that one end of the Qin is scorched.

This Qin was found by Cai Yong.[2] According to the "History of the Later
Han",[3] Cai Yong once heard very loud sound of someone burning Firmiana
simplex[4] wood for cooking and realized that it is a good material for
making Qin soundboards. He then asked for the wood and cut it into a Qin,
and the sound it produced was pleasant to hear as expected. However, one
end of the wood had already been scorched by the time Cai Yong got it,
hence the name "Scorched end".

Seal was named so with the hope that it will be more useful than it seems
and will be discovered by the right people who need the right tool.

[1] http://en.wikipedia.org/wiki/Guqin  
[2] http://en.wikipedia.org/wiki/Cai_Yong  
[3] http://en.wikipedia.org/wiki/History_of_the_Later_Han  
[4] http://en.wikipedia.org/wiki/Firmiana_simplex  
