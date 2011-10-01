/*
 * ruby_binding.c is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License.
 * See COPYING attached with the library.
 */

#include <stdio.h>
#include <stddef.h>
#include <seal.h>
#include "ruby.h"

typedef void* allocator_t(void);
typedef void deallocator_t(void*);
typedef void inputter_t(void*, const char*, seal_fmt_t);
typedef void* getter_t(seal_buf_t*);

static VALUE mAudio;
static VALUE eAudioError;

void
check_seal_err()
{
    seal_err_t err = seal_get_err();

    if (err != SEAL_OK)
        rb_raise(eAudioError, "%s", seal_get_err_msg(err));
}

VALUE
name2sym(const char* name)
{
    return ID2SYM(rb_intern(name));
}

VALUE
alloc(VALUE klass, allocator_t* allocate, deallocator_t* free)
{
    void* obj = allocate();
    check_seal_err();

    return Data_Wrap_Struct(klass, 0, free, obj);
}

seal_fmt_t
map_fmt(VALUE rsym)
{
    if (!NIL_P(rsym))
    {
        rsym = rb_convert_type(rsym, T_SYMBOL, "Symbol", "to_sym");
        if (rsym == name2sym("wav"))
            return SEAL_WAV_FMT;
        else if (rsym == name2sym("ov"))
            return SEAL_OV_FMT;
        else if (rsym == name2sym("mpg"))
            return SEAL_MPG_FMT;
    }

    return SEAL_UNKNOWN_FMT;
}

void
input_audio(int argc, VALUE* argv, void* media, inputter_t* input)
{
    VALUE filename;
    VALUE format;

    rb_scan_args(argc, argv, "11", &filename, &format);
    input(media, rb_string_value_cstr(&filename), map_fmt(format));
    check_seal_err();
}

VALUE
get_buf_attr(VALUE rbuf, getter_t get)
{
    void* attr;

    attr = get(DATA_PTR(rbuf));
    check_seal_err();

    return LONG2FIX(attr);
}

void
extract3float(VALUE rarr, float* x, float* y, float* z)
{
    rarr = rb_convert_type(rarr, T_ARRAY, "Array", "to_a");
    *x = NUM2DBL(rb_ary_entry(rarr, 0));
    *y = NUM2DBL(rb_ary_entry(rarr, 1));
    *z = NUM2DBL(rb_ary_entry(rarr, 2));
}

VALUE
set_src3float(VALUE rsrc, VALUE rarr,
              void (*set)(seal_src_t*, float, float, float))
{
    float x, y, z;

    extract3float(rarr, &x, &y, &z);
    set(DATA_PTR(rsrc), x, y, z);

    return Qnil;
}

VALUE
set_listener3float(VALUE rarr, void (*set)(float, float, float))
{
    float x, y, z;

    extract3float(rarr, &x, &y, &z);
    set(x, y, z);

    return Qnil;
}

void
bulk_convert_float(VALUE* rtuple, float* tuple, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        rtuple[i] = rb_float_new(tuple[i]);
}

VALUE
get_src3float(VALUE rsrc,
                       void (*get)(seal_src_t*, float*, float*, float*))
{
    float tuple[3];
    VALUE rtuple[3];

    get(DATA_PTR(rsrc), tuple, tuple + 1, tuple + 2);
    bulk_convert_float(rtuple, tuple, 3);

    return rb_ary_new4(3, rtuple);
}

VALUE
get_listener3float(void (*get)(float*, float*, float*))
{
    float tuple[3];
    VALUE rtuple[3];

    get(tuple, tuple + 1, tuple + 2);
    bulk_convert_float(rtuple, tuple, 3);

    return rb_ary_new4(3, rtuple);
}

VALUE
set_src_float(VALUE rsrc, VALUE rflt, int (*set)(seal_src_t*, float))
{
    set(DATA_PTR(rsrc), NUM2DBL(rflt));
    check_seal_err();

    return Qnil;
}

VALUE
set_listener_float(VALUE rflt, int (*set)(float))
{
    set(NUM2DBL(rflt));
    check_seal_err();

    return Qnil;
}

VALUE
get_src_float(VALUE rsrc, float (*get)(seal_src_t*))
{
    return rb_float_new(get(DATA_PTR(rsrc)));
}

VALUE 
get_listener_float(float (*get)())
{
    return rb_float_new(get());
}

VALUE
set_src_fixnum(VALUE rsrc, VALUE rfixnum, int (*set)(seal_src_t*, size_t))
{
    set(DATA_PTR(rsrc), NUM2LONG(rfixnum));
    check_seal_err();

    return Qnil;
}

VALUE
get_src_fixnum(VALUE rsrc, size_t (*get)(seal_src_t*))
{
    return LONG2FIX(get(DATA_PTR(rsrc)));
}

VALUE
set_src_bool(VALUE rsrc, VALUE rbool, int (*set)(seal_src_t*, int))
{
    set(DATA_PTR(rsrc), RTEST(rbool));
    check_seal_err();

    return Qnil;
}

VALUE
get_src_bool(VALUE rsrc, int (*get)(seal_src_t*))
{
    return get(DATA_PTR(rsrc)) ? Qtrue : Qfalse;
}

VALUE
src_op(VALUE rsrc, void (*op)(seal_src_t*))
{
    op(DATA_PTR(rsrc));

    return Qnil;
}

seal_stream_t*
extract_stream(VALUE rstream)
{
    return DATA_PTR(rstream);
}

/*
 *  call-seq:
 *      Audio.startup       -> nil
 *      Audio.startup(str)  -> nil
 */
VALUE
startup(int argc, VALUE* argv)
{
    VALUE rstring;

    rb_scan_args(argc, argv, "01", &rstring);
    seal_startup(NIL_P(rstring) ? 0 : rb_string_value_cstr(&rstring));
    check_seal_err();

    return Qnil;
}

/*
 *  call-seq:
 *      Audio.cleanup   -> nil
 */
VALUE
cleanup()
{
    seal_cleanup();

    return Qnil;
}

/*
 *  call-seq:
 *      Audio::Buffer.allocate  -> buffer
 */
VALUE
alloc_buf(VALUE klass)
{
    return alloc(klass, (allocator_t*) seal_alloc_buf,
                 (deallocator_t*) seal_free_buf);
}

/*
 *  call-seq:
 *      Audio::Buffer.new(filename [, format])  -> buffer
 */
VALUE
init_buf(int argc, VALUE* argv, VALUE rbuf)
{
    input_audio(argc, argv, DATA_PTR(rbuf), (inputter_t*) seal_init_buf);

    return rbuf;
}

/*
 *  call-seq:
 *      buffer.load(filename [, format])    -> nil
 */
VALUE 
load_buf(int argc, VALUE* argv, VALUE rbuf)
{
    input_audio(argc, argv, DATA_PTR(rbuf), (inputter_t*) seal_load2buf);

    return Qnil;
}

/*
 *  call-seq:
 *      buffer.size ->  fixnum
 */
VALUE 
get_buf_size(VALUE rbuf)
{
    return get_buf_attr(rbuf, (getter_t*) seal_get_buf_size);
}

/*
 *  call-seq:
 *      buffer.frequency    -> fixnum
 */
VALUE
get_buf_freq(VALUE rbuf)
{
    return get_buf_attr(rbuf, (getter_t*) seal_get_buf_freq);
}

/*
 *  call-seq:
 *      buffer.bit_depth    -> fixnum
 */
VALUE
get_buf_bps(VALUE rbuf)
{
    return get_buf_attr(rbuf, (getter_t*) seal_get_buf_bps);
}

/*
 *  call-seq:
 *      buffer.channel_count    -> fixnum
 */
VALUE
get_buf_nchannels(VALUE rbuf)
{
    return get_buf_attr(rbuf, (getter_t*) seal_get_buf_nchannels);
}

/*
 *  call-seq:
 *      Audio::Stream.allocate  -> stream
 */
VALUE
alloc_stream(VALUE klass)
{
    return alloc(klass, (allocator_t*) seal_alloc_stream,
                 (deallocator_t*) seal_free_stream);
}

/*
 *  call-seq:
 *      Audio::Stream.new(filename [, format])  -> stream
 *      Audio::Stream.open(filename [, format]) -> stream
 */
VALUE
init_stream(int argc, VALUE* argv, VALUE rstream)
{
    input_audio(argc, argv, DATA_PTR(rstream), (inputter_t*) seal_init_stream);

    return rstream;
}

/*
 *  call-seq:
 *      stream.frequency    -> fixnum
 */
VALUE
get_stream_freq(VALUE rstream)
{
    return LONG2FIX(extract_stream(rstream)->attr.freq);
}

/*
 *  call-seq:
 *      stream.bit_depth    -> fixnum
 */
VALUE
get_stream_bps(VALUE rstream)
{
    return LONG2FIX(extract_stream(rstream)->attr.bit_depth);
}

/*
 *  call-seq:
 *      stream.channel_count    -> fixnum
 */
VALUE
get_stream_nchannels(VALUE rstream)
{
    return LONG2FIX(extract_stream(rstream)->attr.nchannels);
}

/*
 *  call-seq:
 *      stream.rewind   -> nil
 */
VALUE
rewind_stream(VALUE rstream)
{
    seal_rewind_stream(DATA_PTR(rstream));

    return Qnil;
}

/*
 *  call-seq:
 *      stream.close    -> nil
 */
VALUE
close_stream(VALUE rstream)
{
    seal_close_stream(DATA_PTR(rstream));
    check_seal_err();

    return Qnil;
}

/*
 *  call-seq:
 *      Audio::Source.allocate  -> source
 */
VALUE
alloc_src(VALUE klass)
{
    return alloc(klass, (allocator_t*) seal_alloc_src, (deallocator_t*) seal_free_src);
}

/*
 *  call-seq:
 *      Audio::Source.new   -> source
 */
VALUE
init_src(VALUE rsrc)
{
    return rsrc;
}

/*
 *  call-seq:
 *      source.play ->  nil
 */
VALUE
play_src(VALUE rsrc)
{
    return src_op(rsrc, seal_play_src);
}

/*
 *  call-seq:
 *      source.pause -> nil
 */
VALUE
pause_src(VALUE rsrc)
{
    return src_op(rsrc, seal_pause_src);
}

/*
 *  call-seq:
 *      source.stop ->  nil
 */
VALUE
stop_src(VALUE rsrc)
{
    return src_op(rsrc, seal_stop_src);
}

/*
 *  call-seq:
 *      source.rewind ->    nil
 */
VALUE
rewind_src(VALUE rsrc)
{
    return src_op(rsrc, seal_rewind_src);
}

/*
 *  call-seq:
 *      source.detach -> nil
 */
VALUE
detach_src_audio(VALUE rsrc)
{
    return src_op(rsrc, seal_detach_src_audio);
}

/*
 *  call-seq:
 *      source.buffer = buffer  -> nil
 */
VALUE
set_src_buf(VALUE rsrc, VALUE rbuf)
{
    seal_buf_t* buf;
    
    Data_Get_Struct(rbuf, seal_buf_t, buf);
    seal_set_src_buf(DATA_PTR(rsrc), buf);
    check_seal_err();
    rb_iv_set(rsrc, "@buffer", rbuf);

    return Qnil;
}

/*
 *  call-seq:
 *      source.buffer ->    buffer
 */
VALUE
get_src_buf(VALUE rsrc)
{
    return rb_iv_get(rsrc, "@buffer");
}

/*
 *  call-seq:
 *      source.stream = stream  -> nil
 */
VALUE
set_src_stream(VALUE rsrc, VALUE rstream)
{
    seal_stream_t* stream;

    Data_Get_Struct(rstream, seal_stream_t, stream);
    seal_set_src_stream(DATA_PTR(rsrc), stream);
    check_seal_err();
    rb_iv_set(rsrc, "@stream", rstream);

    return Qnil;
}

/*
 *  call-seq:
 *      source.stream   -> stream
 */
VALUE
get_src_stream(VALUE rsrc)
{
    return rb_iv_get(rsrc, "@stream");
}

/*
 *  call-seq:
 *      source.update   -> nil
 */
VALUE update_src(VALUE rsrc)
{
    seal_update_src(DATA_PTR(rsrc));
    check_seal_err();

    return Qnil;
}

/*
 *  call-seq:
 *      source.position = [flt, flt, flt]   -> nil
 */
VALUE
set_src_pos(VALUE rsrc, VALUE rarr)
{
    return set_src3float(rsrc, rarr, seal_set_src_pos);
}

/*
 *  call-seq:
 *      source.position -> [flt, flt, flt]
 */
VALUE
get_src_pos(VALUE rsrc)
{
    return get_src3float(rsrc, seal_get_src_pos);
}

/*
 *  call-seq:
 *      source.velocity = [flt, flt, flt]   -> nil
 */
VALUE
set_src_vel(VALUE rsrc, VALUE rarr)
{
    return set_src3float(rsrc, rarr, seal_set_src_vel);
}

/*
 *  call-seq:
 *      source.velocity -> [flt, flt, flt]
 */
VALUE
get_src_vel(VALUE rsrc)
{
    return get_src3float(rsrc, seal_get_src_vel);
}

/*
 *  call-seq:
 *      source.pitch = flt  -> nil
 */
VALUE
set_src_pitch(VALUE rsrc, VALUE pitch)
{
    return set_src_float(rsrc, pitch, seal_set_src_pitch);
}

/*
 *  call-seq:
 *      source.pitch    -> flt
 */
VALUE
get_src_pitch(VALUE rsrc)
{
    return get_src_float(rsrc, seal_get_src_pitch);
}

/*
 *  call-seq:
 *      source.gain = flt   -> nil
 */
VALUE
set_src_gain(VALUE rsrc, VALUE gain)
{
    return set_src_float(rsrc, gain, seal_set_src_gain);
}

/*
 *  call-seq:
 *      source.gain -> flt
 */
VALUE
get_src_gain(VALUE rsrc)
{
    return get_src_float(rsrc, seal_get_src_gain);
}

/*
 *  call-seq:
 *      source.relative = true or false -> nil
 */
VALUE
set_src_relative(VALUE rsrc, VALUE rbool)
{
    return set_src_bool(rsrc, rbool, seal_set_src_relative);
}

/*
 *  call-seq:
 *      source.relative -> true or false
 */
VALUE
is_src_relative(VALUE rsrc)
{
    return get_src_bool(rsrc, seal_is_src_relative);
}

/*
 *  call-seq:
 *      source.looping = true or false  -> nil
 */
VALUE
set_src_looping(VALUE rsrc, VALUE rbool)
{
    return set_src_bool(rsrc, rbool, seal_set_src_looping);
}

/*
 *  call-seq:
 *      source.looping  -> true or false
 */
VALUE
is_src_looping(VALUE rsrc)
{
    return get_src_bool(rsrc, seal_is_src_looping);
}

/*
 *  call-seq:
 *      source.queue_size = fixnum  -> nil
 */
VALUE set_src_queue_size(VALUE rsrc, VALUE size)
{
    return set_src_fixnum(rsrc, size, seal_set_src_queue_size);
}

/*
 *  call-seq:
 *      source.queue_size   -> fixnum
 */
VALUE
get_src_queue_size(VALUE rsrc)
{
    return get_src_fixnum(rsrc, seal_get_src_queue_size);
}

/*
 *  call-seq:
 *      source.chunk_size = fixnum  -> nil
 */
VALUE
set_src_chunk_size(VALUE rsrc, VALUE size)
{
    return set_src_fixnum(rsrc, size, seal_set_src_chunk_size);
}

/*
 *  call-seq:
 *      source.chunk_size   -> fixnum
 */
VALUE
get_src_chunk_size(VALUE rsrc)
{
    return get_src_fixnum(rsrc, seal_get_src_chunk_size);
}

/*
 *  call-seq:
 *      source.type -> :streaming or :static
 */
VALUE
get_src_type(VALUE rsrc)
{
    switch (seal_get_src_type(DATA_PTR(rsrc)))
    {
    case SEAL_STREAMING:
        return name2sym("streaming");
    default:
        return name2sym("static");
    }
}

/*
 *  call-seq:
 *      source.state    -> :playing, :paused, :stopped or :initial
 */
VALUE
get_src_state(VALUE rsrc)
{
    switch (seal_get_src_state(DATA_PTR(rsrc)))
    {
    case SEAL_PLAYING:
        return name2sym("playing");
    case SEAL_PAUSED:
        return name2sym("paused");
    case SEAL_STOPPED:
        return name2sym("stopped");
    default:
        return name2sym("initial");
    }
}

/*
 *  call-seq:
 *      Audio.listener  -> listener
 */
VALUE
get_listener()
{
    return rb_const_get(mAudio, rb_intern("LISTENER"));
}

/*
 *  call-seq:
 *      Audio.listener.gain = flt   -> nil
 */
VALUE
set_listener_gain(VALUE rlistener, VALUE gain)
{
    return set_listener_float(gain, seal_set_listener_gain);
}

/*
 *  call-seq:
 *      Audio.listener.gain -> flt
 */
VALUE
get_listener_gain(VALUE rlistener)
{
    return get_listener_float(seal_get_listener_gain);
}

/*
 *  call-seq:
 *      Audio.listener.position = [flt, flt, flt]   -> nil
 */
VALUE
set_listener_pos(VALUE rlistener, VALUE rarr)
{
    return set_listener3float(rarr, seal_set_listener_pos);
}

/*
 *  call-seq:
 *      Audio.listener.position -> [flt, flt, flt]
 */
VALUE
GetListenerPosition(VALUE rlistener)
{
    return get_listener3float(seal_get_listener_pos);
}

/*
 *  call-seq:
 *      Audio.listener.velocity = [flt, flt, flt]   -> nil
 */
VALUE
set_listener_vel(VALUE rlistener, VALUE rarr)
{
    return set_listener3float(rarr, seal_set_listener_vel);
}

/*
 *  call-seq:
 *      Audio.listener.position -> [flt, flt, flt]
 */
VALUE get_listener_vel(VALUE rlistener)
{
    return get_listener3float(seal_get_listener_vel);
}

/*
 *  call-seq:
 *      Audio.listener.orientation = [[flt, flt, flt], [flt, flt, flt]] -> nil
 */
VALUE
set_listener_orien(VALUE rlistener, VALUE rarr)
{
    float orien[6];

    rarr = rb_convert_type(rarr, T_ARRAY, "Array", "to_a");
    extract3float(rb_ary_entry(rarr, 0), orien, orien + 1, orien + 2);
    extract3float(rb_ary_entry(rarr, 1), orien + 3, orien + 4, orien + 5);
    seal_set_listener_orien(orien);

    return Qnil;
}

/*
 *  call-seq:
 *      Audio.listener.orientation  -> [[flt, flt, flt], [flt, flt, flt]]
 */
VALUE
get_listener_orien(VALUE rlistener)
{
    float tuple[6];
    VALUE rtuple[6];
    VALUE orien[2];

    seal_get_listener_orien(tuple);
    bulk_convert_float(rtuple, tuple, 6);
    orien[0] = rb_ary_new4(3, rtuple);
    orien[1] = rb_ary_new4(3, rtuple + 3);

    return rb_ary_new4(2, orien);
}

void
bind_core(void)
{
    mAudio = rb_define_module("Audio");
    eAudioError = rb_define_class("AudioError", rb_eException);
    rb_define_singleton_method(mAudio, "startup", startup, -1);
    rb_define_singleton_method(mAudio, "cleanup", cleanup, 0);

}

void
bind_buf(void)
{
    VALUE cBuffer = rb_define_class_under(mAudio, "Buffer", rb_cObject);
    rb_define_alloc_func(cBuffer, alloc_buf);
    rb_define_method(cBuffer, "initialize", init_buf, -1);
    rb_define_method(cBuffer, "load", load_buf, -1);
    rb_define_method(cBuffer, "size", get_buf_size, 0);
    rb_define_method(cBuffer, "frequency", get_buf_freq, 0);
    rb_define_method(cBuffer, "bit_depth", get_buf_bps, 0);
    rb_define_method(cBuffer, "channel_count", get_buf_nchannels, 0);
}

void
bind_stream(void)
{
    VALUE cStream = rb_define_class_under(mAudio, "Stream", rb_cObject);
    rb_define_alloc_func(cStream, alloc_stream);
    rb_define_method(cStream, "initialize", init_stream, -1);
    rb_define_method(cStream, "frequency", get_stream_freq, 0);
    rb_define_method(cStream, "bit_depth", get_stream_bps, 0);
    rb_define_method(cStream, "channel_count", get_stream_nchannels, 0);
    rb_define_method(cStream, "rewind", rewind_stream, 0);
    rb_define_method(cStream, "close", close_stream, 0);
    rb_define_alias(rb_singleton_class(cStream), "open", "new");
}

void
bind_src(void)
{
    VALUE cSource = rb_define_class_under(mAudio, "Source", rb_cObject);
    rb_define_alloc_func(cSource, alloc_src);
    rb_define_method(cSource, "initialize", init_src, 0);
    rb_define_method(cSource, "play", play_src, 0);
    rb_define_method(cSource, "pause", pause_src, 0);
    rb_define_method(cSource, "stop", stop_src, 0);
    rb_define_method(cSource, "rewind", rewind_src, 0);
    rb_define_method(cSource, "buffer=", set_src_buf, 1);
    rb_define_method(cSource, "buffer", get_src_buf, 0);
    rb_define_method(cSource, "stream=", set_src_stream, 1);
    rb_define_method(cSource, "stream", get_src_stream, 0);
    rb_define_method(cSource, "update", update_src, 0);
    rb_define_method(cSource, "position=", set_src_pos, 1);
    rb_define_method(cSource, "position", get_src_pos, 0);
    rb_define_method(cSource, "velocity=", set_src_pos, 1);
    rb_define_method(cSource, "velocity", get_src_pos, 0);
    rb_define_method(cSource, "pitch=", set_src_pitch, 1);
    rb_define_method(cSource, "pitch", get_src_pitch, 0);
    rb_define_method(cSource, "gain=", set_src_gain, 1);
    rb_define_method(cSource, "gain", get_src_gain, 0);
    rb_define_method(cSource, "relative=", set_src_relative, 1);
    rb_define_method(cSource, "relative", is_src_relative, 0);
    rb_define_method(cSource, "looping=", set_src_looping, 1);
    rb_define_method(cSource, "looping", is_src_looping, 0);
    rb_define_method(cSource, "queue_size=", set_src_queue_size, 1);
    rb_define_method(cSource, "queue_size", get_src_queue_size, 0);
    rb_define_method(cSource, "chunk_size=", set_src_chunk_size, 1);
    rb_define_method(cSource, "chunk_size", get_src_chunk_size, 0);
    rb_define_method(cSource, "type", get_src_type, 0);
    rb_define_method(cSource, "state", get_src_state, 0);
}

void bind_listener(void)
{
    VALUE cListener = rb_define_class_under(mAudio, "Listener", rb_cObject);
    rb_define_const(mAudio, "LISTENER", rb_data_object_alloc(cListener, 0, 0, 0));
    rb_define_singleton_method(mAudio, "listener", get_listener, 0);
    rb_undef_alloc_func(cListener);
    rb_undef_method(rb_singleton_class(cListener), "new");
    rb_define_method(cListener, "position=", set_listener_pos, 1);
    rb_define_method(cListener, "position", GetListenerPosition, 0);
    rb_define_method(cListener, "velocity=", set_listener_vel, 1);
    rb_define_method(cListener, "velocity", get_listener_vel, 0);
    rb_define_method(cListener, "gain=", set_listener_gain, 1);
    rb_define_method(cListener, "gain", get_listener_gain, 0);
    rb_define_method(cListener, "orientation=", set_listener_orien, 1);
    rb_define_method(cListener, "orientation", get_listener_orien, 0);
}

/* CRuby extension entry point. */
void
Init_seal(void)
{
    bind_core();
    bind_buf();
    bind_stream();
    bind_src();
    bind_listener();
}