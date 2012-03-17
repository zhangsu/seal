/*
 * ruby_binding.c is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License.
 * See COPYING attached with the library.
 */

#include <stdlib.h>
#include <seal.h>
#include "ruby.h"

static VALUE mSeal;
static VALUE eSealError;
static const char WAV_SYMBOL[] = "wav";
static const char OV_SYMBOL[] = "ov";
static const char MPG_SYMBOL[] = "mpg";
static const char UNDETERMINED_SYMBOL[] = "undetermined";
static const char STATIC_SYMBOL[] = "static";
static const char STREAMING_SYMBOL[] = "streaming";
static const char INITIAL_SYMBOL[] = "initial";
static const char PLAYING_SYMBOL[] = "playing";
static const char PAUSED_SYMBOL[] = "paused";
static const char STOPPED_SYMBOL[] = "stopped";

VALUE
name2sym(const char* name)
{
    return ID2SYM(rb_intern(name));
}

static void*
validate_memory(void* memory)
{
    if (memory == 0)
        rb_raise(eSealError, "%s", seal_get_err_msg(SEAL_CANNOT_ALLOC_MEM));
    return memory;
}

void
check_seal_err(seal_err_t err)
{
    if (err != SEAL_OK)
        rb_raise(eSealError, "%s", seal_get_err_msg(err));
}

void
free_obj(void* obj, void *destroy)
{
    ((void (*)(void*)) destroy)(obj);
    free(obj);
}

void
free_src(void* src)
{
    free_obj(src, seal_destroy_src);
}

void
free_buf(void* buf)
{
    free_obj(buf, seal_destroy_buf);
}

void
free_stream(void* stream)
{
    free_obj(stream, seal_close_stream);
}

void
free_reverb(void* reverb)
{
    free_obj(reverb, seal_destroy_reverb);
}

void
free_effect_slot(void* effect_slot)
{
    free_obj(effect_slot, seal_destroy_effect_slot);
}

VALUE
alloc(VALUE klass, size_t size, void* free)
{
    void* obj;

    obj = validate_memory(calloc(1, size));

    return Data_Wrap_Struct(klass, 0, free, obj);
}

seal_fmt_t
map_format(VALUE symbol)
{
    if (NIL_P(symbol))
        return SEAL_UNKNOWN_FMT;

    symbol = rb_convert_type(symbol, T_SYMBOL, "Symbol", "to_sym");
    if (symbol == name2sym(WAV_SYMBOL))
        return SEAL_WAV_FMT;
    else if (symbol == name2sym(OV_SYMBOL))
        return SEAL_OV_FMT;
    else if (symbol == name2sym(MPG_SYMBOL))
        return SEAL_MPG_FMT;
}

void
input_audio(int argc, VALUE* argv, void* media, void* _input)
{
    typedef seal_err_t inputter_t(void*, const char*, seal_fmt_t);

    VALUE filename;
    VALUE format;

    inputter_t* input = (inputter_t*) _input;
    rb_scan_args(argc, argv, "11", &filename, &format);
    check_seal_err(input(media, rb_string_value_ptr(&filename),
                         map_format(format)));
}

VALUE
get_buf_attr(VALUE rbuf, seal_err_t (*get)(seal_buf_t*, int*))
{
    int value;

    check_seal_err(get(DATA_PTR(rbuf), &value));

    return INT2NUM(value);
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
              seal_err_t (*set)(seal_src_t*, float, float, float))
{
    float x, y, z;

    extract3float(rarr, &x, &y, &z);
    check_seal_err(set(DATA_PTR(rsrc), x, y, z));

    return rarr;
}

VALUE
set_listener3float(VALUE rarr, seal_err_t (*set)(float, float, float))
{
    float x, y, z;

    extract3float(rarr, &x, &y, &z);
    check_seal_err(set(x, y, z));

    return rarr;
}

void
convert_bulk_float(VALUE* rtuple, float* tuple, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        rtuple[i] = rb_float_new(tuple[i]);
}

VALUE
get_src3float(VALUE rsrc,
              seal_err_t (*get)(seal_src_t*, float*, float*, float*))
{
    float tuple[3];
    VALUE rtuple[3];

    check_seal_err(get(DATA_PTR(rsrc), tuple, tuple + 1, tuple + 2));
    convert_bulk_float(rtuple, tuple, 3);

    return rb_ary_new4(3, rtuple);
}

VALUE
get_listener3float(seal_err_t (*get)(float*, float*, float*))
{
    float tuple[3];
    VALUE rtuple[3];

    check_seal_err(get(tuple, tuple + 1, tuple + 2));
    convert_bulk_float(rtuple, tuple, 3);

    return rb_ary_new4(3, rtuple);
}

VALUE
set_src_float(VALUE rsrc, VALUE rflt, seal_err_t (*set)(seal_src_t*, float))
{
    check_seal_err(set(DATA_PTR(rsrc), NUM2DBL(rflt)));

    return rflt;
}

VALUE
set_listener_float(VALUE rflt, seal_err_t (*set)(float))
{
    check_seal_err(set(NUM2DBL(rflt)));

    return rflt;
}

VALUE
get_src_float(VALUE rsrc, seal_err_t (*get)(seal_src_t*, float*))
{
    float value;

    check_seal_err(get(DATA_PTR(rsrc), &value));

    return rb_float_new(value);
}

VALUE
get_listener_float(seal_err_t (*get)(float*))
{
    float value;

    get(&value);

    return rb_float_new(value);
}

VALUE
set_src_fixnum(VALUE rsrc, VALUE rfixnum,
               seal_err_t (*set)(seal_src_t*, size_t))
{
    check_seal_err(set(DATA_PTR(rsrc), NUM2ULONG(rfixnum)));

    return rfixnum;
}

VALUE
get_src_fixnum(VALUE rsrc, seal_err_t (*get)(seal_src_t*, size_t*))
{
    size_t size;

    check_seal_err(get(DATA_PTR(rsrc), &size));

    return ULONG2NUM(size);
}

VALUE
set_src_bool(VALUE rsrc, VALUE rbool, seal_err_t (*set)(seal_src_t*, char))
{
    check_seal_err(set(DATA_PTR(rsrc), RTEST(rbool)));

    return rbool;
}

VALUE
get_src_bool(VALUE rsrc, seal_err_t (*get)(seal_src_t*, char*))
{
    char bool;

    check_seal_err(get(DATA_PTR(rsrc), &bool));

    return bool ? Qtrue : Qfalse;
}

VALUE
src_op(VALUE rsrc, seal_err_t (*op)(seal_src_t*))
{
    check_seal_err(op(DATA_PTR(rsrc)));

    return rsrc;
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
    check_seal_err(seal_startup(NIL_P(rstring)
                   ? 0 : rb_string_value_ptr(&rstring)));

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
 *      Seal::Buffer.allocate   -> buffer
 */
VALUE
alloc_buf(VALUE klass)
{
    return alloc(klass, sizeof (seal_buf_t), seal_destroy_buf);
}

/*
 *  call-seq:
 *      Seal::Buffer.new(filename [, format])   -> buffer
 */
VALUE
init_buf(int argc, VALUE* argv, VALUE rbuf)
{
    seal_buf_t* buf;

    buf = DATA_PTR(rbuf);
    check_seal_err(seal_init_buf(buf));
    input_audio(argc, argv, buf, seal_load2buf);

    return rbuf;
}

/*
 *  call-seq:
 *      Seal::Buffer.load(filename [, format])   -> buffer
 */
VALUE
load_buf(int argc, VALUE* argv, VALUE rbuf)
{
    input_audio(argc, argv, DATA_PTR(rbuf), seal_load2buf);

    return rbuf;
}

/*
 *  call-seq:
 *      buffer.size ->  fixnum
 */
VALUE
get_buf_size(VALUE rbuf)
{
    return get_buf_attr(rbuf, seal_get_buf_size);
}

/*
 *  call-seq:
 *      buffer.frequency    -> fixnum
 */
VALUE
get_buf_freq(VALUE rbuf)
{
    return get_buf_attr(rbuf, seal_get_buf_freq);
}

/*
 *  call-seq:
 *      buffer.bit_depth    -> fixnum
 */
VALUE
get_buf_bps(VALUE rbuf)
{
    return get_buf_attr(rbuf, seal_get_buf_bps);
}

/*
 *  call-seq:
 *      buffer.channel_count    -> fixnum
 */
VALUE
get_buf_nchannels(VALUE rbuf)
{
    return get_buf_attr(rbuf, seal_get_buf_nchannels);
}

/*
 *  call-seq:
 *      Seal::Stream.allocate   -> stream
 */
VALUE
alloc_stream(VALUE klass)
{
    return alloc(klass, sizeof (seal_stream_t), seal_close_stream);
}

/*
 *  call-seq:
 *      Seal::Stream.new(filename [, format])   -> stream
 *      Seal::Stream.open(filename [, format])  -> stream
 */
VALUE
init_stream(int argc, VALUE* argv, VALUE rstream)
{
    input_audio(argc, argv, DATA_PTR(rstream), seal_open_stream);

    return rstream;
}

/*
 *  call-seq:
 *      stream.frequency    -> fixnum
 */
VALUE
get_stream_freq(VALUE rstream)
{
    return INT2NUM(extract_stream(rstream)->attr.freq);
}

/*
 *  call-seq:
 *      stream.bit_depth    -> fixnum
 */
VALUE
get_stream_bps(VALUE rstream)
{
    return INT2NUM(extract_stream(rstream)->attr.bit_depth);
}

/*
 *  call-seq:
 *      stream.channel_count    -> fixnum
 */
VALUE
get_stream_nchannels(VALUE rstream)
{
    return INT2NUM(extract_stream(rstream)->attr.nchannels);
}

/*
 *  call-seq:
 *      stream.rewind   -> stream
 */
VALUE
rewind_stream(VALUE rstream)
{
    seal_rewind_stream(DATA_PTR(rstream));

    return rstream;
}

/*
 *  call-seq:
 *      stream.close    -> stream
 */
VALUE
close_stream(VALUE rstream)
{
    check_seal_err(seal_close_stream(DATA_PTR(rstream)));

    return rstream;
}

/*
 *  call-seq:
 *      Seal::Source.allocate -> source
 */
VALUE
alloc_src(VALUE klass)
{
    return alloc(klass, sizeof (seal_src_t), seal_destroy_src);
}

/*
 *  call-seq:
 *      Seal::Source.new  -> source
 */
VALUE
init_src(VALUE rsrc)
{
    check_seal_err(seal_init_src(DATA_PTR(rsrc)));
    return rsrc;
}

/*
 *  call-seq:
 *      source.play ->  source
 */
VALUE
play_src(VALUE rsrc)
{
    return src_op(rsrc, seal_play_src);
}

/*
 *  call-seq:
 *      source.pause -> source
 */
VALUE
pause_src(VALUE rsrc)
{
    return src_op(rsrc, seal_pause_src);
}

/*
 *  call-seq:
 *      source.stop ->  source
 */
VALUE
stop_src(VALUE rsrc)
{
    return src_op(rsrc, seal_stop_src);
}

/*
 *  call-seq:
 *      source.rewind ->    source
 */
VALUE
rewind_src(VALUE rsrc)
{
    return src_op(rsrc, seal_rewind_src);
}

/*
 *  call-seq:
 *      source.detach -> source
 */
VALUE
detach_src_audio(VALUE rsrc)
{
    return src_op(rsrc, seal_detach_src_audio);
}

/*
 *  call-seq:
 *      source.buffer = buffer  -> buffer
 */
VALUE
set_src_buf(VALUE rsrc, VALUE rbuf)
{
    seal_buf_t* buf;

    Data_Get_Struct(rbuf, seal_buf_t, buf);
    check_seal_err(seal_set_src_buf(DATA_PTR(rsrc), buf));
    rb_iv_set(rsrc, "@buffer", rbuf);

    return rbuf;
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
 *      source.stream = stream  -> stream
 */
VALUE
set_src_stream(VALUE rsrc, VALUE rstream)
{
    seal_stream_t* stream;

    Data_Get_Struct(rstream, seal_stream_t, stream);
    check_seal_err(seal_set_src_stream(DATA_PTR(rsrc), stream));
    rb_iv_set(rsrc, "@stream", rstream);

    return rstream;
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
 *      source.update   -> source
 */
VALUE update_src(VALUE rsrc)
{
    return src_op(rsrc, seal_update_src);
}

/*
 *  call-seq:
 *      source.position = [flt, flt, flt]   -> [flt, flt, flt]
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
 *      source.velocity = [flt, flt, flt]   -> [flt, flt, flt]
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
 *      source.pitch = flt  -> [flt, flt, flt]
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
 *      source.gain = flt   -> [flt, flt, flt]
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
 *      source.relative = true or false -> true or false
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
 *      source.looping = true or false  -> true or false
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
 *      source.queue_size = fixnum  -> true or false
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
 *      source.chunk_size = fixnum  -> true or false
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
    seal_src_type_t type;

    check_seal_err(seal_get_src_type(DATA_PTR(rsrc), &type));
    switch (type) {
    case SEAL_STATIC:
        return name2sym(STATIC_SYMBOL);
    case SEAL_STREAMING:
        return name2sym(STREAMING_SYMBOL);
    default:
        return name2sym(UNDETERMINED_SYMBOL);
    }
}

/*
 *  call-seq:
 *      source.state    -> :playing, :paused, :stopped or :initial
 */
VALUE
get_src_state(VALUE rsrc)
{
    seal_src_state_t state;

    check_seal_err(seal_get_src_state(DATA_PTR(rsrc), &state));
    switch (state) {
    case SEAL_PLAYING:
        return name2sym(PLAYING_SYMBOL);
    case SEAL_PAUSED:
        return name2sym(PAUSED_SYMBOL);
    case SEAL_STOPPED:
        return name2sym(STOPPED_SYMBOL);
    default:
        return name2sym(INITIAL_SYMBOL);
    }
}

/*
 *  call-seq:
 *      Audio.listener  -> listener
 */
VALUE
get_listener()
{
    return rb_const_get(mSeal, rb_intern("LISTENER"));
}

/*
 *  call-seq:
 *      Audio.listener.gain = flt   -> [flt, flt, flt]
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
 *      Audio.listener.position = [flt, flt, flt]   -> [flt, flt, flt]
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
 *      Audio.listener.velocity = flt, flt, flt   -> [flt, flt, flt]
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
VALUE
get_listener_vel(VALUE rlistener)
{
    return get_listener3float(seal_get_listener_vel);
}

/*
 *  call-seq:
 *      Audio.listener.orientation = [flt, flt, flt], [flt, flt, flt]
 *          -> [[flt, flt, flt], [flt, flt, flt]]
 */
VALUE
set_listener_orien(VALUE rlistener, VALUE rarr)
{
    float orien[6];

    rarr = rb_convert_type(rarr, T_ARRAY, "Array", "to_a");
    extract3float(rb_ary_entry(rarr, 0), orien, orien + 1, orien + 2);
    extract3float(rb_ary_entry(rarr, 1), orien + 3, orien + 4, orien + 5);
    check_seal_err(seal_set_listener_orien(orien));

    return rarr;
}

/*
 *  call-seq:
 *      Audio.listener.orientation  -> [[flt, flt, flt], [flt, flt, flt]]
 *
 *  Examples:
 *      at, up = Audio.listener.orientation
 *      at    # => the `at' vector [flt, flt, flt]
 *      up    # => the `up' vector [flt, flt, flt]
 *      (at_x, at_y, ay_z), (up_x, up_y, up_z) = Audio.listener.orientation
 *      at_x  # => the x component of the `at' vector
 *      # ...
 *      up_z  # => the z component of the `up' vector
 */
VALUE
get_listener_orien(VALUE rlistener)
{
    float tuple[6];
    VALUE rtuple[6];
    VALUE orien[2];

    check_seal_err(seal_get_listener_orien(tuple));
    convert_bulk_float(rtuple, tuple, 6);
    orien[0] = rb_ary_new4(3, rtuple);
    orien[1] = rb_ary_new4(3, rtuple + 3);

    return rb_ary_new4(2, orien);
}

void
bind_core(void)
{
    mSeal = rb_define_module("Seal");
    eSealError = rb_define_class("SealError", rb_eException);
    rb_define_singleton_method(mSeal, "startup", startup, -1);
    rb_define_singleton_method(mSeal, "cleanup", cleanup, 0);

}

void
bind_buf(void)
{
    VALUE cBuffer = rb_define_class_under(mSeal, "Buffer", rb_cObject);
    rb_define_alloc_func(cBuffer, alloc_buf);
    rb_define_method(cBuffer, "initialize", init_buf, -1);
    rb_define_method(cBuffer, "size", get_buf_size, 0);
    rb_define_method(cBuffer, "frequency", get_buf_freq, 0);
    rb_define_method(cBuffer, "bit_depth", get_buf_bps, 0);
    rb_define_method(cBuffer, "channel_count", get_buf_nchannels, 0);
    rb_define_alias(cBuffer, "load", "initialize");
}

void
bind_stream(void)
{
    VALUE cStream = rb_define_class_under(mSeal, "Stream", rb_cObject);
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
    VALUE cSource = rb_define_class_under(mSeal, "Source", rb_cObject);
    rb_define_alloc_func(cSource, alloc_src);
    rb_define_method(cSource, "initialize", init_src, 0);
    rb_define_method(cSource, "play", play_src, 0);
    rb_define_method(cSource, "pause", pause_src, 0);
    rb_define_method(cSource, "stop", stop_src, 0);
    rb_define_method(cSource, "rewind", rewind_src, 0);
    rb_define_method(cSource, "detach", detach_src_audio, 0);
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
    rb_define_method(cSource, "relative?", is_src_relative, 0);
    rb_define_method(cSource, "looping=", set_src_looping, 1);
    rb_define_method(cSource, "looping?", is_src_looping, 0);
    rb_define_method(cSource, "queue_size=", set_src_queue_size, 1);
    rb_define_method(cSource, "queue_size", get_src_queue_size, 0);
    rb_define_method(cSource, "chunk_size=", set_src_chunk_size, 1);
    rb_define_method(cSource, "chunk_size", get_src_chunk_size, 0);
    rb_define_method(cSource, "type", get_src_type, 0);
    rb_define_method(cSource, "state", get_src_state, 0);
}

void
bind_listener(void)
{
    VALUE cListener = rb_define_class_under(mSeal, "Listener", rb_cObject);
    VALUE listener = rb_data_object_alloc(cListener, 0, 0, 0);
    rb_define_const(mSeal, "LISTENER", listener);
    rb_define_singleton_method(mSeal, "listener", get_listener, 0);
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
