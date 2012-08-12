/*
 * ruby_binding.c is part of the Scorched End Audio Library (SEAL) and is
 * licensed under the terms of the GNU Lesser General Public License.
 * See COPYING attached with the library.
 */

#include <stdlib.h>
#include <seal.h>
#include "ruby.h"

static const char WAV_SYM[] = "wav";
static const char OV_SYM[] = "ov";
static const char MPG_SYM[] = "mpg";

static const char UNDETERMINED_SYM[] = "undetermined";
static const char STATIC_SYM[] = "static";
static const char STREAMING_SYM[] = "streaming";

static const char INITIAL_SYM[] = "initial";
static const char PLAYING_SYM[] = "playing";
static const char PAUSED_SYM[] = "paused";
static const char STOPPED_SYM[] = "stopped";

static VALUE mSeal;
static VALUE eSealError;

#define DEFINE_ALLOCATOR(obj)                                               \
static VALUE                                                                \
alloc_##obj(VALUE klass)                                                    \
{                                                                           \
    return alloc(klass, sizeof (seal_##obj##_t), free_##obj);               \
}

#define DEFINE_DEALLOCATOR(obj)                                             \
static void                                                                 \
free_##obj(void* obj)                                                       \
{                                                                           \
    free_obj(obj, seal_destroy_##obj);                                      \
}

static VALUE
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

static void
check_seal_err(seal_err_t err)
{
    if (err != SEAL_OK)
        rb_raise(eSealError, "%s", seal_get_err_msg(err));
}

static void
free_obj(void* obj, void *destroy)
{
    ((seal_err_t (*)(void*)) destroy)(obj);
    free(obj);
}

DEFINE_DEALLOCATOR(src)
DEFINE_DEALLOCATOR(buf)
DEFINE_DEALLOCATOR(rvb)
DEFINE_DEALLOCATOR(efs)

static void
free_stream(void* stream)
{
    free_obj(stream, seal_close_stream);
}

static VALUE
alloc(VALUE klass, size_t size, void* free)
{
    void* obj;

    obj = validate_memory(calloc(1, size));

    return Data_Wrap_Struct(klass, 0, free, obj);
}

DEFINE_ALLOCATOR(src)
DEFINE_ALLOCATOR(buf)
DEFINE_ALLOCATOR(stream)
DEFINE_ALLOCATOR(rvb)
DEFINE_ALLOCATOR(efs)

static void
extract_3float(VALUE rarr, float* x, float* y, float* z)
{
    rarr = rb_convert_type(rarr, T_ARRAY, "Array", "to_a");
    *x = NUM2DBL(rb_ary_entry(rarr, 0));
    *y = NUM2DBL(rb_ary_entry(rarr, 1));
    *z = NUM2DBL(rb_ary_entry(rarr, 2));
}

static void
convert_bulk_float(VALUE* rtuple, float* tuple, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        rtuple[i] = rb_float_new(tuple[i]);
}

static VALUE
set_obj_float(VALUE robj, VALUE rflt, void* set)
{
    check_seal_err(((seal_err_t (*)(void*, float)) set)(
        DATA_PTR(robj), NUM2DBL(rflt)
    ));

    return rflt;
}

static VALUE
set_obj_int(VALUE robj, VALUE rnum, void* set)
{
    check_seal_err(((seal_err_t (*)(void*, int)) set)(
        DATA_PTR(robj), NUM2INT(rnum)
    ));
}

static VALUE
set_obj_ulong(VALUE robj, VALUE rnum, void* set)
{
    check_seal_err(((seal_err_t (*)(void*, int)) set)(
        DATA_PTR(robj), NUM2ULONG(rnum)
    ));
}

static VALUE
set_obj_char(VALUE robj, VALUE rbool, void* set)
{
    check_seal_err(((seal_err_t (*)(void*, char)) set)(
        DATA_PTR(robj), RTEST(rbool)
    ));

    return rbool;
}

static VALUE
set_obj_3float(VALUE robj, VALUE rarr, void* set)
{
    float x, y, z;

    extract_3float(rarr, &x, &y, &z);
    check_seal_err(((seal_err_t (*)(void*, float, float, float)) set)(
        DATA_PTR(robj), x, y, z
    ));

    return rarr;
}

static void
get_obj_attr(VALUE robj, void* pvalue, void* get)
{
    check_seal_err(((seal_err_t (*)(void*, void*)) get)(
        DATA_PTR(robj), pvalue
    ));
}

static VALUE
get_obj_float(VALUE robj, void* get)
{
    float flt;

    get_obj_attr(robj, &flt, get);

    return rb_float_new(flt);
}

static VALUE
get_obj_int(VALUE robj, void* get)
{
    int integer;

    get_obj_attr(robj, &integer, get);

    return INT2NUM(integer);
}

static VALUE
get_obj_ulong(VALUE robj, void* get)
{
    unsigned long long_integer;

    get_obj_attr(robj, &long_integer, get);

    return ULONG2NUM(long_integer);
}

static VALUE
get_obj_char(VALUE robj, void* get)
{
    char bool;

    get_obj_attr(robj, &bool, get);

    return bool ? Qtrue : Qfalse;
}

static VALUE
get_obj_3float(VALUE robj, void* get)
{
    float tuple[3];
    VALUE rtuple[3];

    check_seal_err(((seal_err_t (*)(void*, float*, float*, float*)) get)(
        DATA_PTR(robj), tuple, tuple + 1, tuple + 2
    ));
    convert_bulk_float(rtuple, tuple, 3);

    return rb_ary_new4(3, rtuple);
}

static void
define_enum(VALUE mModule, const char* name, int e)
{
    rb_define_const(mModule, name, INT2NUM(e));
}

static seal_fmt_t
map_format(VALUE symbol)
{
    if (NIL_P(symbol))
        return SEAL_UNKNOWN_FMT;

    symbol = rb_convert_type(symbol, T_SYMBOL, "Symbol", "to_sym");
    if (symbol == name2sym(WAV_SYM))
        return SEAL_WAV_FMT;
    else if (symbol == name2sym(OV_SYM))
        return SEAL_OV_FMT;
    else if (symbol == name2sym(MPG_SYM))
        return SEAL_MPG_FMT;
    else
        return SEAL_UNKNOWN_FMT;
}

static void
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

static VALUE
set_listener_3float(VALUE rarr, seal_err_t (*set)(float, float, float))
{
    float x, y, z;

    extract_3float(rarr, &x, &y, &z);
    check_seal_err(set(x, y, z));

    return rarr;
}

static VALUE
get_listener_3float(seal_err_t (*get)(float*, float*, float*))
{
    float tuple[3];
    VALUE rtuple[3];

    check_seal_err(get(tuple, tuple + 1, tuple + 2));
    convert_bulk_float(rtuple, tuple, 3);

    return rb_ary_new4(3, rtuple);
}

static VALUE
set_listener_float(VALUE rflt, seal_err_t (*set)(float))
{
    check_seal_err(set(NUM2DBL(rflt)));

    return rflt;
}

static VALUE
get_listener_float(seal_err_t (*get)(float*))
{
    float value;

    check_seal_err(get(&value));

    return rb_float_new(value);
}

static VALUE
src_op(VALUE rsrc, seal_err_t (*op)(seal_src_t*))
{
    check_seal_err(op(DATA_PTR(rsrc)));

    return rsrc;
}

static seal_stream_t*
extract_stream(VALUE rstream)
{
    return DATA_PTR(rstream);
}

/*
 *  call-seq:
 *      Seal.startup       -> nil
 *      Seal.startup(str)  -> nil
 *
 * Initializes Seal by specifying the device name _str_. This function is
 * not re-entrant nor thread-safe and should be called only once per Seal
 * session. Match a call to <code>seal_startup</code> with a call to
 * <code>seal_cleanup</code> and never call <code>seal_starup</code> twice in
 * a row.
 */
static VALUE
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
 *      Seal.cleanup   -> nil
 *
 * Uninitializes Seal and invalidate all Seal objects. Thread-unsafe.
 */
static VALUE
cleanup()
{
    seal_cleanup();

    return Qnil;
}

/*
 *  call-seq:
 *      Seal.per_source_effect_limit    -> fixnum
 *
 * Returns the maximum number of effect slots a source can feed concurrently.
 */
static VALUE
per_source_effect_limit()
{
    return INT2NUM(seal_get_per_src_effect_limit());
}

/*
 *  call-seq:
 *      Seal::Buffer.new(filename [, format])   -> buffer
 *
 * Initializes a new buffer and loads it with audio from _filename_. _format_
 * specifies the format of the audio file; automatic recognition of the audio
 * format will be attempted if _format_ is not specified. See Seal::Format for
 * possible values. Sets all the attributes appropriately.
 */
static VALUE
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
 *      buffer.load(filename [, format])   -> buffer
 *
 * Loads audio from _filename_ to _buffer_ which must not be currently used by
 * any source. Sets all the attributes appropriately. _format_ specifies the
 * format of the audio file; automatic recognition of the audio format will be
 * attempted if _format_ is not specified. See Seal::Format for possible
 * values.Sets all the attributes appropriately.
 */
static VALUE
load_buf(int argc, VALUE* argv, VALUE rbuf)
{
    input_audio(argc, argv, DATA_PTR(rbuf), seal_load2buf);

    return rbuf;
}

/*
 *  call-seq:
 *      buffer.size ->  fixnum
 *
 * Gets the size, in bytes, of _buffer_. The default is 0.
 */
static VALUE
get_buf_size(VALUE rbuf)
{
    return get_obj_int(rbuf, seal_get_buf_size);
}

/*
 *  call-seq:
 *      buffer.frequency    -> fixnum
 *
 * Gets the frequency (sample rate) of the audio contained in _buffer_. The
 * default is 0 when the buffer is not loaded.
 */
static VALUE
get_buf_freq(VALUE rbuf)
{
    return get_obj_int(rbuf, seal_get_buf_freq);
}

/*
 *  call-seq:
 *      buffer.bit_depth    -> fixnum
 *
 * Gets the bit depth (bits per sample) of the audio contained in _buffer_.
 * The default is 16.
 */
static VALUE
get_buf_bps(VALUE rbuf)
{
    return get_obj_int(rbuf, seal_get_buf_bps);
}

/*
 *  call-seq:
 *      buffer.channel_count    -> fixnum
 *
 * Gets the number of channels of the audio contained in _buffer_. The default
 * is 1.
 */
static VALUE
get_buf_nchannels(VALUE rbuf)
{
    return get_obj_int(rbuf, seal_get_buf_nchannels);
}

/*
 *  call-seq:
 *      Seal::Stream.new(filename [, format])   -> stream
 *      Seal::Stream.open(filename [, format])  -> stream
 *
 * Opens a audio stream from _filename_. _format_ specifies the format of the
 * audio file; automatic recognition of the audio format will be attempted if
 * _format_ is nil. See Seal::Format for possible values.
 */
static VALUE
init_stream(int argc, VALUE* argv, VALUE rstream)
{
    input_audio(argc, argv, DATA_PTR(rstream), seal_open_stream);

    return rstream;
}

/*
 *  call-seq:
 *      stream.frequency    -> fixnum
 *
 * Gets the frequency (sample rate) of the audio contained in _streamed_. The
 * default is 0 when the stream is not opened.
 */
static VALUE
get_stream_freq(VALUE rstream)
{
    return INT2NUM(extract_stream(rstream)->attr.freq);
}

/*
 *  call-seq:
 *      stream.bit_depth    -> fixnum
 *
 * Gets the bit depth (bits per sample) of the audio contained in _stream_.
 * The default is 16.
 */
static VALUE
get_stream_bps(VALUE rstream)
{
    return INT2NUM(extract_stream(rstream)->attr.bit_depth);
}

/*
 *  call-seq:
 *      stream.channel_count    -> fixnum
 *
 * Gets the number of channels of the audio contained in _stream_. The default
 * is 1.
 */
static VALUE
get_stream_nchannels(VALUE rstream)
{
    return INT2NUM(extract_stream(rstream)->attr.nchannels);
}

/*
 *  call-seq:
 *      stream.rewind   -> stream
 *
 * Rewinds _stream_ to the beginning.
 */
static VALUE
rewind_stream(VALUE rstream)
{
    seal_rewind_stream(DATA_PTR(rstream));

    return rstream;
}

/*
 *  call-seq:
 *      stream.close    -> stream
 *
 * Closes _stream_ which must not be used by any source.
 */
static VALUE
close_stream(VALUE rstream)
{
    check_seal_err(seal_close_stream(DATA_PTR(rstream)));

    return rstream;
}

/*
 *  call-seq:
 *      Seal::Source.new  -> source
 *
 * Initializes a new source.
 */
static VALUE
init_src(VALUE rsrc)
{
    check_seal_err(seal_init_src(DATA_PTR(rsrc)));

    return rsrc;
}

/*
 *  call-seq:
 *      source.play ->  source
 *
 * Starts to play _source_. Applying to a playing source will restart the
 * playback from the beginning thus reset the sampling offset too. If the
 * source is a streaming source, restarting the playback will automatically
 * rewind the stream to the beginning. Applying to an initial or stopped
 * source will start start playing and change its state to State::PLAYING.
 * Applying to a paused source will resume playing and change its state to
 * State::PLAYING.
 */
static VALUE
play_src(VALUE rsrc)
{
    return src_op(rsrc, seal_play_src);
}

/*
 *  call-seq:
 *      source.pause -> source
 *
 * Pauses the playing of _source_. Applying to a playing source will change
 * its state to State::PAUSED. Applying to an initial, paused or stopped
 * source has no effect.
 */
static VALUE
pause_src(VALUE rsrc)
{
    return src_op(rsrc, seal_pause_src);
}

/*
 *  call-seq:
 *      source.stop ->  source
 *
 * Stops the playing of _source_. Applying to a playing or paused source will
 * change its state to State::STOPPED. Applying to an initial or stopped
 * source has no effect. Resets the sampling offset.
 */
static VALUE
stop_src(VALUE rsrc)
{
    return src_op(rsrc, seal_stop_src);
}

/*
 *  call-seq:
 *      source.rewind ->    source
 *
 * Rewinds _source_ to the beginning. Applying to a playing, paused or stopped
 * source will change its state to State::INITIAL. Applying to an initial
 * source has no effect. The sampling offset will be reset to the beginning.
 * Other attributes are preserved.
 */
static VALUE
rewind_src(VALUE rsrc)
{
    return src_op(rsrc, seal_rewind_src);
}

/*
 *  call-seq:
 *      source.detach -> source
 *
 * Releases the current buffer or stream from _source_ (hence empties the
 * queue for streaming sources). Will reset the source to Type::UNDETERMINED
 * and the source state to State::STOPPED. Will not free the associated buffer
 * or stream.
 */
static VALUE
detach_src_audio(VALUE rsrc)
{
    return src_op(rsrc, seal_detach_src_audio);
}

/*
 *  call-seq:
 *      source.buffer = buffer  -> buffer
 *
 * Associates _buffer_ with _source_ so that the source is ready to play the
 * audio contained in _buffer_. Can be applied only to sources in the initial
 * or stopped states and that are not of streaming type. If successful, the
 * source will become or remain as Type::STATIC.
 */
static VALUE
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
 *
 * Gets the buffer of _source_. The default is nil.
 */
static VALUE
get_src_buf(VALUE rsrc)
{
    return rb_iv_get(rsrc, "@buffer");
}

/*
 *  call-seq:
 *      source.stream = stream  -> stream
 *
 * Associates (opened) _stream_ with _source_ so that audio data can be
 * continuously fetched from a file rather than loading everything to memory
 * at once. Can be applied to sources in any playing state but not on static
 * sources. When replacing an attached stream, the new stream must have the
 * same audio format as the old one. Also be aware of the fact that in this
 * case there could still be some chunks of the old stream at the front of the
 * streaming queue waiting to be played. If successful, the source will become
 * or remain as Type::STREAMING. The streaming queue will be filled after this
 * call returns; after the queue starts to be played, #update should be called
 * to refill the queue.
 */
static VALUE
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
 *
 * Gets the stream of _source_. The default is nil.
 */
static VALUE
get_src_stream(VALUE rsrc)
{
    return rb_iv_get(rsrc, "@stream");
}

/*
 *  call-seq:
 *      source.update   -> source
 *
 * Updates _source_. If _source_ is not up-to-date, the playback will end
 * before the end of the stream is reached. Does nothing if _source_ is not a
 * streaming source. Also does nothing if auto update is on.
 */
static VALUE update_src(VALUE rsrc)
{
    return src_op(rsrc, seal_update_src);
}

/*
 *  call-seq:
 *      source.position = [flt, flt, flt]   -> [flt, flt, flt]
 *
 * Sets the position of _source_ in a right-handed Cartesian coordinate
 * system. Use of NaN and infinity is undefined. 
 */
static VALUE
set_src_pos(VALUE rsrc, VALUE value)
{
    return set_obj_3float(rsrc, value, seal_set_src_pos);
}

/*
 *  call-seq:
 *      source.position -> [flt, flt, flt]
 *
 * Gets the position of _source_. The default is ( 0.0, 0.0, 0.0 ).
 */
static VALUE
get_src_pos(VALUE rsrc)
{
    return get_obj_3float(rsrc, seal_get_src_pos);
}

/*
 *  call-seq:
 *      source.velocity = [flt, flt, flt]   -> [flt, flt, flt]
 *
 * Sets the velocity of _source_ in a right-handed Cartesian coordinate
 * system. The velocity of the source does not affect its position but is a
 * factor used during the Doppler effect emulation. Use of NaN is undefined.
 *
 */
static VALUE
set_src_vel(VALUE rsrc, VALUE value)
{
    return set_obj_3float(rsrc, value, seal_set_src_vel);
}

/*
 *  call-seq:
 *      source.velocity -> [flt, flt, flt]
 *
 * Gets the velocity of _source_. The default is ( 0.0, 0.0, 0.0 ).
 */
static VALUE
get_src_vel(VALUE rsrc)
{
    return get_obj_3float(rsrc, seal_get_src_vel);
}

/*
 *  call-seq:
 *      source.pitch = flt  -> [flt, flt, flt]
 *
 * Sets the pitch shift multiplier of _source_. 1.0 means identity; each
 * reduction by 1/2 means a pitch shift of -12 semitones; each doubling means
 * a pitch shift of 12 semitones. Use of 0.0 is undefined.
 */
static VALUE
set_src_pitch(VALUE rsrc, VALUE value)
{
    return set_obj_float(rsrc, value, seal_set_src_pitch);
}

/*
 *  call-seq:
 *      source.pitch    -> flt
 *
 * Gets the pitch of _source_. The default is 1.0.
 */
static VALUE
get_src_pitch(VALUE rsrc)
{
    return get_obj_float(rsrc, seal_get_src_pitch);
}

/*
 *  call-seq:
 *      source.gain = flt   -> [flt, flt, flt]
 *
 * Sets the scalar amplitude multiplier of _source_. 1.0 means that the sound
 * is unattenuated; 0.5 means an attenuation of 6 dB; 0.0 means silence.
 */
static VALUE
set_src_gain(VALUE rsrc, VALUE value)
{
    return set_obj_float(rsrc, value, seal_set_src_gain);
}

/*
 *  call-seq:
 *      source.gain -> flt
 *
 * Gets the gain of _source_. The default is 1.0.
 */
static VALUE
get_src_gain(VALUE rsrc)
{
    return get_obj_float(rsrc, seal_get_src_gain);
}

/*
 *  call-seq:
 *      source.auto = true or false -> true or false
 *
 * Sets whether _source_ should be automatically updated asynchronously by a
 * background thread. If this thread is running, user calls to #update does
 * nothing. If auto update is disabled after it is enabled, it will take
 * effect the next time the source gets played.
 */
static VALUE
set_src_auto(VALUE rsrc, VALUE value)
{
    return set_obj_char(rsrc, value, seal_set_src_auto);
}

/*
 *  call-seq:
 *      source.auto     -> true or false
 *
 * Determines if _source_ is automatically updated. The default is true.
 */
static VALUE
is_src_auto(VALUE rsrc)
{
    return get_obj_char(rsrc, seal_is_src_auto);
}

/*
 *  call-seq:
 *      source.relative = true or false -> true or false
 *
 * Sets whether _source_'s position, velocity, cone and direction are all
 * relative to the listener position.
 */
static VALUE
set_src_relative(VALUE rsrc, VALUE value)
{
    return set_obj_char(rsrc, value, seal_set_src_relative);
}

/*
 *  call-seq:
 *      source.relative   -> true or false
 *
 * Determines if _source_ is relative. The default is false.
 */
static VALUE
is_src_relative(VALUE rsrc)
{
    return get_obj_char(rsrc, seal_is_src_relative);
}

/*
 *  call-seq:
 *      source.looping = true or false  -> true or false
 *
 * Sets whether the playback of _source_ is looping. A looping source will
 * never enter State::STOPPED; it will immediate enter State::INITIAL and then
 * State::Playing after it reaches the end of the last buffer.
 */
static VALUE
set_src_looping(VALUE rsrc, VALUE value)
{
    return set_obj_char(rsrc, value, seal_set_src_looping);
}

/*
 *  call-seq:
 *      source.looping  -> true or false
 *
 * Determines if _source_ is looping. The default is false (0).
 */
static VALUE
is_src_looping(VALUE rsrc)
{
    return get_obj_char(rsrc, seal_is_src_looping);
}

/*
 *  call-seq:
 *      source.queue_size = fixnum  -> true or false
 *
 * Sets the size of the streaming queue internally used by _source_. The queue
 * maintains a multiple buffering mechanism when streaming the audio data.
 * Multiple bufferring allows buffers in the queue to be processed while the
 * one at the front of the queue is still being played. A queue of size 2
 * (double buffering) may still be inefficient in CPU-, and I/O-bound
 * situations while triple, or even quad buffering generally produces better
 * sound quality in non-memory-bound situations. _fixnum_ must be in the
 * interval [2, 127]; an out-of-bound value will be adjusted to the closest
 * bound automatically.
 */
static VALUE
set_src_queue_size(VALUE rsrc, VALUE value)
{
    return set_obj_float(rsrc, value, seal_set_src_queue_size);
}

/*
 *  call-seq:
 *      source.queue_size   -> fixnum
 *
 * Gets the size, in byte, of _source_'s streaming queue. The default is 3.
 */
static VALUE
get_src_queue_size(VALUE rsrc)
{
    return get_obj_float(rsrc, seal_get_src_queue_size);
}

/*
 *  call-seq:
 *      source.chunk_size = fixnum  -> true or false
 *
 * Sets the maximum size, in byte, of the audio chunk which buffers the audio
 * data constantly fetched from an audio stream. Using small chunks may cause
 * playback to occur before the required audio chunk is ready, which in turn
 * causes unexpected stop of playback. _fixnum_ must be in the interval
 * \[9216, 16773120] and must be a multiple of 9216; non-multiple value will be
 * adjusted to the closest smaller multiple automatically.
 */
static VALUE
set_src_chunk_size(VALUE rsrc, VALUE value)
{
    return set_obj_int(rsrc, value, seal_set_src_chunk_size);
}

/*
 *  call-seq:
 *      source.chunk_size   -> fixnum
 *
 * Gets the size, in byte, of _source_'s streaming chunk. The default is
 * 36864.
 */
static VALUE
get_src_chunk_size(VALUE rsrc)
{
    return get_obj_int(rsrc, seal_get_src_chunk_size);
}
/*
 *  call-seq:
 *      source.type -> :streaming or :static
 *
 * Gets the type of _source_.
 */
static VALUE
get_src_type(VALUE rsrc)
{
    seal_src_type_t type;

    check_seal_err(seal_get_src_type(DATA_PTR(rsrc), &type));
    switch (type) {
    case SEAL_STATIC:
        return name2sym(STATIC_SYM);
    case SEAL_STREAMING:
        return name2sym(STREAMING_SYM);
    default:
        return name2sym(UNDETERMINED_SYM);
    }
}

/*
 *  call-seq:
 *      source.state    -> :playing, :paused, :stopped or :initial
 *
 * Gets the state of _source_.
 */
static VALUE
get_src_state(VALUE rsrc)
{
    seal_src_state_t state;

    check_seal_err(seal_get_src_state(DATA_PTR(rsrc), &state));
    switch (state) {
    case SEAL_PLAYING:
        return name2sym(PLAYING_SYM);
    case SEAL_PAUSED:
        return name2sym(PAUSED_SYM);
    case SEAL_STOPPED:
        return name2sym(STOPPED_SYM);
    default:
        return name2sym(INITIAL_SYM);
    }
}

/*
 *  call-seq:
 *      reverb.load(preset) -> reverb
 *
 * Loads the specified reverb paramter preset into _reverb_. 
 */
static VALUE
load_rvb(VALUE rrvb, VALUE rpreset)
{
    seal_rvb_t* rvb;

    Data_Get_Struct(rrvb, seal_rvb_t, rvb);
    check_seal_err(seal_load_rvb(DATA_PTR(rrvb), rpreset));

    return rrvb;
}

/*
 *  call-seq:
 *      Seal::Reverb.new            -> reverb
 *      Seal::Reverb.new(preset)    -> reverb
 *
 * Initializes a new reverb effect. If a preset is specified, initializes
 * the reverb object to load the preset.
 */
static VALUE
init_rvb(int argc, VALUE* argv, VALUE rrvb)
{
    seal_rvb_t* rvb;
    VALUE rpreset;

    rvb = DATA_PTR(rrvb);
    check_seal_err(seal_init_rvb(rvb));

    rb_scan_args(argc, argv, "01", &rpreset);
    if (!NIL_P(rpreset))
        load_rvb(rrvb, rpreset);

    return rrvb;
}

/*
 *  call-seq:
 *      reverb.density = flt  -> flt
 *
 * Sets the modal density of _reverb_ in the interval [0.0, 1.0]. The
 * density controls the coloration of the late reverb. The Lower the value,
 * the more coloration.
 */
static VALUE
set_src_density(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_density);
}

/*
 *  call-seq:
 *      reverb.density  -> flt
 *
 * Gets the density of _reverb_. The default is 1.0.
 */
static VALUE
get_rvb_density(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_density);
}

/*
 *  call-seq:
 *      reverb.diffusion = flt  -> flt
 *
 * Sets the diffusion of _reverb_ in the interval [0.0, 1.0]. The diffusion
 * controls the echo density in the reverberation decay. Reducing diffusion
 * gives the reverberation a more "grainy" character that is especially
 * noticeable with percussive sound sources. If you set a diffusion value of
 * 0.0, the later reverberation sounds like a succession of distinct echoes.
 */
static VALUE
set_rvb_diffusion(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_diffusion);
}

/*
 *  call-seq:
 *      reverb.diffusion  -> flt
 *
 * Gets the diffusion of _reverb_. The default is 1.0.
 */
static VALUE
get_rvb_diffusion(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_diffusion);
}

/*
 *  call-seq:
 *      reverb.gain = flt  -> flt
 *
 * Sets the gain of _reverb_ in the interval [0.0, 1.0], or from -100 dB (no
 * reflected sound at all) to 0 dB (the maximum amount). The gain is the
 * master volume control for the reflected sound (both early reflections and
 * reverberation) that the reverb effect adds to all sources. It sets the
 * maximum amount of reflections and reverberation added to the final sound
 * mix.
 */
static VALUE
set_rvb_gain(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_gain);
}

/*
 *  call-seq:
 *      reverb.gain  -> flt
 *
 * Gets the gain of _reverb_. The default is 0.32f.
 */
static VALUE
get_rvb_gain(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_gain);
}

/*
 *  call-seq:
 *      reverb.hfgain = flt  -> flt
 *
 * Sets the high-frequency gain of _reverb_ in the interval [0.0, 1.0], or
 * from -100 dB (virtually no reflected sound) to 0 dB (no filter). The high-
 * frequency gain further tweaks reflected sound by attenuating it at high
 * frequencies. It controls a low-pass filter that applies globally to the
 * reflected sound of all sound sources feeding the particular instance of the
 * reverb effect.
 */
static VALUE
set_rvb_hfgain(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_hfgain);
}

/*
 *  call-seq:
 *      reverb.hfgain  -> flt
 *
 * Gets the high-frequency gain of _reverb_. The default is 0.89f.
 */
static VALUE
get_rvb_hfgain(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_hfgain);
}

/*
 *  call-seq:
 *      reverb.decay_time = flt  -> flt
 *
 * Sets the decay time of _reverb_ in the interval [0.1, 20.0], typically
 * from a small room with very dead surfaces to a large room with very live
 * surfaces.
 */
static VALUE
set_rvb_decay_time(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_decay_time);
}

/*
 *  call-seq:
 *      reverb.decay_time  -> flt
 *
 * Gets the decay time of _reverb_. The default is 1.49f.
 */
static VALUE
get_rvb_decay_time(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_decay_time);
}

/*
 *  call-seq:
 *      reverb.hfdecay_ratio = flt  -> flt
 *
 * Sets the high-frequency decay ratio, or the spectral quality of the decay
 * time of _reverb_ in the interval [0.1, 20.0]. It is the ratio of high-
 * frequency decay time relative to the time set by decay Time. 1.0 means
 * neutral: the decay time is equal for all frequencies. As this value
 * increases above 1.0, the high-frequency decay time increases so it's longer
 * than the decay time at low frequencies. You hear a more brilliant
 * reverberation with a longer decay at high frequencies. As this value
 * decreases below 1.0, the high-frequency decay time decreases so it's
 * shorter than the decay time of the low frequencies. You hear a more natural
 * reverberation.
 */
static VALUE
set_rvb_hfdecay_ratio(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_hfdecay_ratio);
}

/*
 *  call-seq:
 *      reverb.hfdecay_ratio  -> flt
 *
 * Gets the high-frequency decay ratio of _reverb_. The default is 0.83f.
 */
static VALUE
get_rvb_hfdecay_ratio(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_hfdecay_ratio);
}

/*
 *  call-seq:
 *      reverb.reflections_gain = flt  -> flt
 *
 * Sets the reflections gain, or the overall amount of initial reflections
 * relative to the gain of _reverb_ in the interval [0.0, 3.16f], or from
 * -100 dB (no initial reflections at all) to 10 dB. The reflections gain is
 * corrected by the value of the gain property and does not affect the
 * subsequent reverberation decay.
 *
 * You can increase the amount of initial reflections to simulate a more
 * narrow space or closer walls, especially effective if you associate the
 * initial reflections increase with a reduction in reflections delays by
 * lowering the value of the reflection delay property. To simulate open or
 * semi-open environments, you can maintain the amount of early reflections
 * while reducing the value of the late gain property, which controls later
 * reflections.
 */
static VALUE
set_rvb_reflections_gain(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_reflections_gain);
}

/*
 *  call-seq:
 *      reverb.reflections_gain  -> flt
 *
 * Gets the reflections gain of _reverb_. The default is 0.05f.
 */
static VALUE
get_rvb_reflections_gain(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_reflections_gain);
}

/*
 *  call-seq:
 *      reverb.reflections_delay = flt  -> flt
 *
 * Sets the reflections delay of _reverb_ in the interval [0.0, 0.3] (in
 * seconds). It is the amount of delay between the arrival time of the direct
 * path from the source to the first reflection from the source. You can
 * reduce or increase this delay to simulate closer or more distant reflective
 * surfaces and therefore control the perceived size of the room.
 */
static VALUE
set_rvb_reflections_delay(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_reflections_delay);
}

/*
 *  call-seq:
 *      reverb.reflections_delay  -> flt
 *
 * Gets the reflections delay of _reverb_. The default is 0.007f.
 */
static VALUE
get_rvb_reflections_delay(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_reflections_delay);
}

/*
 *  call-seq:
 *      reverb.late_gain = flt  -> flt
 *
 * Sets the late gain, or the overall amount of later reverberation relative
 * to the gain of _reverb_ in the interval [0.0, 10.0], or from -100 dB (no
 * late reverberation at all) to 20 dB.
 *
 * Note that late gain and decay time are independent properties: if you
 * adjust decay time without changing late gain, the total intensity (the
 * averaged square of the amplitude) of the late reverberation remains
 * constant.
 */
static VALUE
set_rvb_late_gain(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_late_gain);
}

/*
 *  call-seq:
 *      reverb.late_gain  -> flt
 *
 * Gets the late gain of _reverb_. The default is 1.26f.
 */
static VALUE
get_rvb_late_gain(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_late_gain);
}

/*
 *  call-seq:
 *      reverb.late_delay = flt  -> flt
 *
 * Sets the late delay of _reverb_ in the interval [0.0, 0.1] (in second)
 * It defines the begin time of the late reverberation relative to the time of
 * the initial reflection (the first of the early reflections). Reducing or
 * increasing late delay is useful for simulating a smaller or larger room.
 */
static VALUE
set_rvb_late_delay(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_late_delay);
}

/*
 *  call-seq:
 *      reverb.late_delay  -> flt
 *
 * Gets the late delay of _reverb_. The default is 0.011f.
 */
static VALUE
get_rvb_late_delay(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_late_delay);
}
/*
 *  call-seq:
 *      reverb.air_absorbtion_hfgain = flt  -> flt
 *
 * Sets the air absorption high-frequency gain of _reverb_ in the interval
 * [0.892, 1.0]. It controls the distance-dependent attenuation at high
 * frequencies caused by the propagation medium. It applies to reflected sound
 * only. You can use this value to simulate sound transmission through foggy
 * air, dry air, smoky atmosphere, and so on. The default value 0.994
 * (-0.05 dB) per meter, which roughly corresponds to typical condition of
 * atmospheric humidity, temperature, and so on. Lowering the value simulates
 * a more absorbent medium (more humidity in the air, for example); raising
 * the value simulates a less absorbent medium (dry desert air, for example).
 */
static VALUE
set_rvb_air_absorbtion_hfgain(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_air_absorbtion_hfgain);
}

/*
 *  call-seq:
 *      reverb.air_absorbtion_hfgain  -> flt
 *
 * Gets the air absorbtion high-frequency gain of _reverb_. The default is
 */
static VALUE
get_rvb_air_absorbtion_hfgain(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_air_absorbtion_hfgain);
}

/*
 *  call-seq:
 *      reverb.room_rolloff_factor = flt  -> flt
 *
 * Sets the room rolloff factor of _reverb_ in the interval [0.0, 10.0]. It
 * is one of two methods available to attenuate the reflected sound
 * (containing both reflections and reverberation) according to source-
 * listener distance. It is defined the same way as the global rolloff factor,
 * but operates on reverb sound instead of direct-path sound. Setting the room
 * rolloff factor value to 1.0 specifies that the reflected sound will decay
 * by 6 dB every time the distance doubles. Any value other than 1.0 is
 * equivalent to a scaling factor applied to the quantity specified by
 * ((source listener distance) - (Reference Distance)). Reference Distance is
 * an OpenAL source parameter that specifies the inner border for distance
 * rolloff effects: if the source comes closer to the listener than the
 * reference distance, the direct-path sound isn't increased as the source
 * comes closer to the listener, and neither is the reflected sound.
 *
 * The default value of Room Rolloff Factor is 0.0 because, by default, the
 * reverb effect naturally manages the reflected sound level automatically for
 * each sound source to simulate the natural rolloff of reflected sound vs.
 * distance in typical rooms. (Note that this isn't the case if the source
 * property @TODO is set to false.) You can use this value as an option to
 * automatic control so you can exaggerate or replace the default
 * automatically-controlled rolloff.
 */
static VALUE
set_rvb_room_rolloff_factor(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_room_rolloff_factor);
}

/*
 *  call-seq:
 *      reverb.room_rolloff_factor  -> flt
 *
 * Gets the room rolloff factor of _reverb_. The default is 0.0.
 */
static VALUE
get_rvb_room_rolloff_factor(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_room_rolloff_factor);
}

/*
 *  call-seq:
 *      reverb.hfdecay_limited = true or false  -> true or false
 *
 * Sets whether the high-frequency decay time automatically stays below a
 * limit value that's derived from the setting of the air absorption high-
 * frequency gain. This limit applies regardless of the setting of the
 * decay high-frequency ratio, and the limit doesn't affect the value of decay
 * high-frequency ratio. This limit, when on, maintains a natural sounding
 * reverberation decay by allowing you to increase the value of decay time
 * without the risk of getting an unnaturally long decay time at high
 * frequencies. If this flag is set to false, high-frequency decay time isn't
 * automatically limited.
 */
static VALUE
set_rvb_hfdecay_limited(VALUE rrvb, VALUE value)
{
    return set_obj_char(rrvb, value, seal_set_rvb_hfdecay_limited);
}

/*
 *  call-seq:
 *      reverb.hfdecay_limited  -> true or false
 *
 * Determines if the high-frequency decay of _reverb_ is limited. The default
 * is true.
 */
static VALUE
is_rvb_hfdecay_limited(VALUE rrvb)
{
    return get_obj_char(rrvb, seal_is_rvb_hfdecay_limited);
}

/*
 *  call-seq:
 *      effect_slot.effect = effect -> effect
 *
 * Fills _effect_slot_ with _effect_, then _effect_slot_ will become ready to
 * feed sources. Pass nil to unfill the slot.
 */
static VALUE
set_efs_effect(VALUE rslot, VALUE reffect)
{
    void* effect;
    seal_err_t err;

    if (NIL_P(reffect)) {
        err = seal_set_efs_effect(DATA_PTR(rslot), 0);
    } else {
        Data_Get_Struct(reffect, void*, effect);
        err = seal_set_efs_effect(DATA_PTR(rslot), effect);
    }
    check_seal_err(err);
    rb_iv_set(rslot, "@effect", reffect);

    return reffect;
}

/*
 *  call-seq:
 *      EffectSlot.new          -> effect_slot
 *      EffectSlot.new(effect)  -> effect_slot
 *
 * Initializes a new effect slot. If an effect object is specified,
 * initializes the effect slot to have that effect object associated.
 */
static VALUE
init_efs(int argc, VALUE* argv, VALUE rslot)
{
    VALUE reffect;

    rb_scan_args(argc, argv, "01", &reffect);
    check_seal_err(seal_init_efs(DATA_PTR(rslot)));
    if (!NIL_P(reffect))
        set_efs_effect(rslot, reffect);

    return rslot;
}

/*
 *  call-seq:
 *      effect_slot.effect  -> effect
 *
 * Gets the effect object in _effect_slot_. The default is nil.
 */
static VALUE
get_efs_effect(VALUE rslot)
{
    return rb_iv_get(rslot, "@effect");
}

/*
 *  call-seq:
 *      slot.feed(index, source)    -> slot
 *
 * Mixes a sound effect loaded into _effect_slot_ with _source_'s output.
 * Later calls to this function with a different effect slot and the same
 * index will override the old effect slot association.
 */
static VALUE
feed_efs(VALUE rslot, VALUE rindex, VALUE rsrc)
{
    seal_src_t* src;

    Data_Get_Struct(rsrc, seal_src_t, src);
    check_seal_err(seal_feed_efs(DATA_PTR(rslot), NUM2INT(rindex), src));

    return rslot;
}

/*
 *  call-seq:
 *      effect_slot.gain = flt  -> flt
 *
 * Sets the output level of _effect_slot_ in the interval [0.0, 1.0]. A
 * value of 0.0 mutes the output.
 */
static VALUE
set_efs_gain(VALUE refs, VALUE value)
{
    return set_obj_float(refs, value, seal_set_efs_gain);
}

/*
 *  call-seq:
 *      effect_slot.gain    -> flt
 *
 * Gets the output level of _effect_slot_ in the interval. The default is
 * 1.0.
 */
static VALUE
get_efs_gain(VALUE refs)
{
    return get_obj_float(refs, seal_get_efs_gain);
}

/*
 *  call-seq:
 *      effect_slot.auto = true or false    -> true or false
 *
 * Sets whether the effect should have automatic adjustments based on the
 * physical positions of the sources and the listener.
 */
static VALUE
set_efs_auto(VALUE refs, VALUE value)
{
    return set_obj_char(refs, value, seal_set_efs_auto);
}

/*
 *  call-seq:
 *      effect_slot.auto    -> true or false
 *
 * Determines if the effect is automatically adjusted. The default is true
 * (nonzero).
 */
static VALUE
is_efs_auto(VALUE refs)
{
    return get_obj_char(refs, seal_is_efs_auto);
}

/*
 *  call-seq:
 *      Seal.listener  -> listener
 *
 * Gets the singleton Listener instance.
 */
static VALUE
get_listener()
{
    return rb_const_get(mSeal, rb_intern("LISTENER"));
}

/*
 *  call-seq:
 *      Seal.listener.gain = flt   -> [flt, flt, flt]
 *
 * Sets the master scalar amplitude multiplier of the listener which applies
 * to all the sources. 1.0 means that the sound is unattenuated; 0.5 means
 * an attenuation of 6 dB; 0.0 means silence.
 */
static VALUE
set_listener_gain(VALUE rlistener, VALUE value)
{
    return set_listener_float(value, seal_set_listener_gain);
}

/*
 *  call-seq:
 *      Seal.listener.gain -> flt
 *
 * Gets the gain of the listener. The default is 1.0.
 */
static VALUE
get_listener_gain(VALUE rlistener, VALUE value)
{
    return get_listener_float(seal_get_listener_gain);
}

/*
 *  call-seq:
 *      Seal.listener.position = [flt, flt, flt]   -> [flt, flt, flt]
 *
 * Sets the position of the listener in a right-handed Cartesian coordinate
 * system. Use of NaN and infinity is undefined.
 */
static VALUE
set_listener_pos(VALUE rlistener, VALUE value)
{
    return set_listener_3float(value, seal_set_listener_pos);
}

/*
 *  call-seq:
 *      Seal.listener.position -> [flt, flt, flt]
 *
 * Gets the position of the listener. The default is ( 0.0, 0.0, 0.0 ).
 */
static VALUE
get_listener_pos(VALUE rlistener, VALUE value)
{
    return get_listener_3float(seal_get_listener_pos);
}

/*
 *  call-seq:
 *      Seal.listener.velocity = flt, flt, flt   -> [flt, flt, flt]
 *
 * Sets the velocity of the listener in a right-handed Cartesian coordinate
 * system. The velocity of the listener does not affect its position but is a
 * factor used during the Doppler effect emulation.
 */
static VALUE
set_listener_vel(VALUE rlistener, VALUE value)
{
    return set_listener_3float(value, seal_set_listener_vel);
}


/*
 *  call-seq:
 *      Seal.listener.velocity -> [flt, flt, flt]
 *
 * Gets the velocity of the listener. The default is ( 0.0, 0.0, 0.0 ).
 */
static VALUE
get_listener_vel(VALUE rlistener, VALUE value)
{
    return get_listener_3float(seal_get_listener_vel);
}

/*
 *  call-seq:
 *      Seal.listener.orientation = [flt, flt, flt], [flt, flt, flt]
 *          -> [[flt, flt, flt], [flt, flt, flt]]
 *
 * Sets the orientation of the listener. The argument must be a pair of
 * 3-tuple consisting of an 'at' vector and an 'up' vector, where the 'at'
 * vector represents the 'forward' direction of the listener and the 'up'
 * vector represents the 'up' direction for the listener. These two vectors
 * must be linearly independent, must not be NaN and must not be normalized.
 * Otherwise, the operation is undefined.
 */
static VALUE
set_listener_orien(VALUE rlistener, VALUE rarr)
{
    float orien[6];

    rarr = rb_convert_type(rarr, T_ARRAY, "Array", "to_a");
    extract_3float(rb_ary_entry(rarr, 0), orien, orien + 1, orien + 2);
    extract_3float(rb_ary_entry(rarr, 1), orien + 3, orien + 4, orien + 5);
    check_seal_err(seal_set_listener_orien(orien));

    return rarr;
}

/*
 *  call-seq:
 *      Seal.listener.orientation  -> [[flt, flt, flt], [flt, flt, flt]]
 *
 * Gets the orientation of the listener. The default is
 * ((0.0, 0.0, -1.0), (0.0, 1.0, 0.0)).
 *
 * Examples:
 *      # at references the `at' vector [flt, flt, flt]
 *      # up references the `up' vector [flt, flt, flt]
 *      at, up = Seal.listener.orientation
 *      # at_x references the x component of the `at' vector
 *      # ...
 *      # up_z references the z component of the `up' vector
 *      (at_x, at_y, ay_z), (up_x, up_y, up_z) = Seal.listener.orientation
 */
static VALUE
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

static void
singletonify(VALUE klass)
{
    rb_undef_alloc_func(klass);
    rb_undef_method(rb_singleton_class(klass), "new");
}

/*
 * Document-module: Seal::Format
 *
 * A collection of supported audio formats.
 */

/*
 * Document-class:  Seal::SealError
 *
 * The Seal Exception class.
 */

static void
bind_core(void)
{
    VALUE mFormat;
    mSeal = rb_define_module("Seal");
    eSealError = rb_define_class_under(mSeal, "SealError", rb_eException);
    rb_define_singleton_method(mSeal, "startup", startup, -1);
    rb_define_singleton_method(mSeal, "cleanup", cleanup, 0);
    rb_define_singleton_method(mSeal, "per_source_effect_limit",
                               per_source_effect_limit, 0);
    /* A string indicating the version of Seal. */
    rb_define_const(mSeal, "VERSION", rb_str_new2(seal_get_version()));
    mFormat = rb_define_module_under(mSeal, "Format");
    /* WAVE format. */
    rb_define_const(mFormat, "WAV", name2sym(WAV_SYM));
    /* Ogg Vorbis format. */
    rb_define_const(mFormat, "OV", name2sym(OV_SYM));
    /* MPEG audio format. */
    rb_define_const(mFormat, "MPG", name2sym(MPG_SYM));
}

/*
 * Document-class:  Seal::Buffer
 *
 * Buffers are essentially abstract representations of the (raw) audio data
 * and are used by sources. Buffers are most suitable for small-sized sound
 * effect which can be efficiently loaded to memory at once. Streams, on the
 * other hand, are more suitable for long audio such as background music.
 */
static void
bind_buf(void)
{
    VALUE cBuffer = rb_define_class_under(mSeal, "Buffer", rb_cObject);
    rb_define_alloc_func(cBuffer, alloc_buf);
    rb_define_method(cBuffer, "initialize", init_buf, -1);
    rb_define_method(cBuffer, "load", load_buf, -1);
    rb_define_method(cBuffer, "size", get_buf_size, 0);
    rb_define_method(cBuffer, "frequency", get_buf_freq, 0);
    rb_define_method(cBuffer, "bit_depth", get_buf_bps, 0);
    rb_define_method(cBuffer, "channel_count", get_buf_nchannels, 0);
}

/*
 * Document-class:  Seal::Stream
 *
 * Streams are used by streaming sources to avoid loading big audio into
 * memory. It is the front end for various decoders.
 */
static void
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

/*
 * Document-class:  Seal::Source
 *
 * Sources are abstract representations of sound sources which emit sound in
 * Euclidean space.
 */

/*
 * Document-module: Seal::Source::State
 *
 * A collection of Source states.
 */

/*
 * Document-module: Seal::Source::Type
 *
 * A collection of Source types.
 */
static void
bind_src(void)
{
    VALUE mType, mState;
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
    rb_define_method(cSource, "auto=", set_src_auto, 1);
    rb_define_method(cSource, "auto", is_src_auto, 0);
    rb_define_alias(cSource, "auto?", "auto");
    rb_define_method(cSource, "relative=", set_src_relative, 1);
    rb_define_method(cSource, "relative", is_src_relative, 0);
    rb_define_alias(cSource, "relative?", "relative");
    rb_define_method(cSource, "looping=", set_src_looping, 1);
    rb_define_method(cSource, "looping", is_src_looping, 0);
    rb_define_alias(cSource, "looping?", "looping");
    rb_define_method(cSource, "queue_size=", set_src_queue_size, 1);
    rb_define_method(cSource, "queue_size", get_src_queue_size, 0);
    rb_define_method(cSource, "chunk_size=", set_src_chunk_size, 1);
    rb_define_method(cSource, "chunk_size", get_src_chunk_size, 0);
    rb_define_method(cSource, "type", get_src_type, 0);
    rb_define_method(cSource, "state", get_src_state, 0);

    mState = rb_define_module_under(cSource, "State");
    /* Indicates a source is in initial state. */
    rb_define_const(mState, "INITIAL", name2sym(INITIAL_SYM));
    /* Indicates a source is playing. */
    rb_define_const(mState, "PLAYING", name2sym(PLAYING_SYM));
    /* Indicates a source is paused from playing. */
    rb_define_const(mState, "PAUSED", name2sym(PAUSED_SYM));
    /* Indicates a source is stopped from playing. */
    rb_define_const(mState, "STOPPED", name2sym(STOPPED_SYM));

    mType = rb_define_module_under(cSource, "Type");
    /* Sources with no audio attached. */
    rb_define_const(mType, "UNDETERMINED", name2sym(UNDETERMINED_SYM));
    /* Sources with an audio buffer attached. */
    rb_define_const(mType, "STATIC", name2sym(STATIC_SYM));
    /* Sources with an audio stream attached. */
    rb_define_const(mType, "STREAMING", name2sym(STREAMING_SYM));
}

/*
 * Document-class:  Seal::Reverb
 *
 * A Reverb object is a set of parameters that define a reverberation effect.
 * Effect objects can be put into an effect slot for sources to use.
 */
static void
bind_rvb(void)
{
    VALUE cReverb = rb_define_class_under(mSeal, "Reverb", rb_cObject);
    VALUE mPreset = rb_define_module_under(cReverb, "Preset");
    VALUE mCastle = rb_define_module_under(mPreset, "Castle");
    VALUE mFactory = rb_define_module_under(mPreset, "Factory");
    VALUE mIcePalace = rb_define_module_under(mPreset, "IcePalace");
    VALUE mSpaceStation = rb_define_module_under(mPreset, "SpaceStation");
    VALUE mWoodenGalleon = rb_define_module_under(mPreset, "WoodenGalleon");
    VALUE mSports = rb_define_module_under(mPreset, "Sports");
    VALUE mPrefab = rb_define_module_under(mPreset, "Prefab");
    VALUE mDome = rb_define_module_under(mPreset, "Dome");
    VALUE mPipe = rb_define_module_under(mPreset, "Pipe");
    VALUE mOutdoors = rb_define_module_under(mPreset, "Outdoors");
    VALUE mMood = rb_define_module_under(mPreset, "Mood");
    VALUE mDriving = rb_define_module_under(mPreset, "Driving");
    VALUE mCity = rb_define_module_under(mPreset, "City");
    VALUE mMisc = rb_define_module_under(mPreset, "Misc");
    rb_define_alloc_func(cReverb, alloc_rvb);
    rb_define_method(cReverb, "initialize", init_rvb, -1);
    rb_define_method(cReverb, "load", load_rvb, 2);
    rb_define_method(cReverb, "density=", set_rvb_diffusion, 1);
    rb_define_method(cReverb, "density", get_rvb_diffusion, 0);
    rb_define_method(cReverb, "diffusion=", set_rvb_diffusion, 1);
    rb_define_method(cReverb, "diffusion", get_rvb_diffusion, 0);
    rb_define_method(cReverb, "gain=", set_rvb_gain, 1);
    rb_define_method(cReverb, "gain", get_rvb_gain, 0);
    rb_define_method(cReverb, "hfgain=", set_rvb_hfgain, 1);
    rb_define_method(cReverb, "hfgain", get_rvb_hfgain, 0);
    rb_define_method(cReverb, "decay_time=", set_rvb_decay_time, 1);
    rb_define_method(cReverb, "decay_time", get_rvb_decay_time, 0);
    rb_define_method(cReverb, "hfdecay_ratio=", set_rvb_hfdecay_ratio, 1);
    rb_define_method(cReverb, "hfdecay_ratio", get_rvb_hfdecay_ratio, 0);
    rb_define_method(cReverb, "reflections_gain=",
                     set_rvb_reflections_gain, 1);
    rb_define_method(cReverb, "reflections_gain",
                     get_rvb_reflections_gain, 0);
    rb_define_method(cReverb, "reflections_delay=",
                     set_rvb_reflections_delay, 1);
    rb_define_method(cReverb, "reflections_delay",
                     get_rvb_reflections_delay, 0);
    rb_define_method(cReverb, "late_gain=", set_rvb_late_gain, 1);
    rb_define_method(cReverb, "late_gain", get_rvb_late_gain, 0);
    rb_define_method(cReverb, "late_delay=", set_rvb_late_delay, 1);
    rb_define_method(cReverb, "late_delay", get_rvb_late_delay, 0);
    rb_define_method(cReverb, "air_absorbtion_hfgain=",
                     set_rvb_air_absorbtion_hfgain, 1);
    rb_define_method(cReverb, "air_absorbtion_hfgain",
                     get_rvb_air_absorbtion_hfgain, 0);
    rb_define_method(cReverb, "room_rolloff_factor=",
                     set_rvb_room_rolloff_factor, 1);
    rb_define_method(cReverb, "room_rolloff_factor",
                     get_rvb_room_rolloff_factor, 0);
    rb_define_method(cReverb, "hfdecay_limited=",
                     set_rvb_hfdecay_limited, 1);
    rb_define_method(cReverb, "hfdecay_limited",
                     is_rvb_hfdecay_limited, 0);
    rb_define_alias(cReverb, "hfdecay_limited?", "hfdecay_limited");

    define_enum(mPreset, "GENERIC", SEAL_GENERIC_REVERB);
    define_enum(mPreset, "PADDEDCELL", SEAL_PADDEDCELL_REVERB);
    define_enum(mPreset, "ROOM", SEAL_ROOM_REVERB);
    define_enum(mPreset, "BATHROOM", SEAL_BATHROOM_REVERB);
    define_enum(mPreset, "LIVINGROOM", SEAL_LIVINGROOM_REVERB);
    define_enum(mPreset, "STONEROOM", SEAL_STONEROOM_REVERB);
    define_enum(mPreset, "AUDITORIUM", SEAL_AUDITORIUM_REVERB);
    define_enum(mPreset, "CONCERTHALL", SEAL_CONCERTHALL_REVERB);
    define_enum(mPreset, "CAVE", SEAL_CAVE_REVERB);
    define_enum(mPreset, "ARENA", SEAL_ARENA_REVERB);
    define_enum(mPreset, "HANGAR", SEAL_HANGAR_REVERB);
    define_enum(mPreset, "CARPETEDHALLWAY", SEAL_CARPETEDHALLWAY_REVERB);
    define_enum(mPreset, "HALLWAY", SEAL_HALLWAY_REVERB);
    define_enum(mPreset, "STONECORRIDOR", SEAL_STONECORRIDOR_REVERB);
    define_enum(mPreset, "ALLEY", SEAL_ALLEY_REVERB);
    define_enum(mPreset, "FOREST", SEAL_FOREST_REVERB);
    define_enum(mPreset, "CITY", SEAL_CITY_REVERB);
    define_enum(mPreset, "MOUNTAINS", SEAL_MOUNTAINS_REVERB);
    define_enum(mPreset, "QUARRY", SEAL_QUARRY_REVERB);
    define_enum(mPreset, "PLAIN", SEAL_PLAIN_REVERB);
    define_enum(mPreset, "PARKINGLOT", SEAL_PARKINGLOT_REVERB);
    define_enum(mPreset, "SEWERPIPE", SEAL_SEWERPIPE_REVERB);
    define_enum(mPreset, "UNDERWATER", SEAL_UNDERWATER_REVERB);
    define_enum(mPreset, "DRUGGED", SEAL_DRUGGED_REVERB);
    define_enum(mPreset, "DIZZY", SEAL_DIZZY_REVERB);
    define_enum(mPreset, "PSYCHOTIC", SEAL_PSYCHOTIC_REVERB);
    define_enum(mCastle, "SMALLROOM", SEAL_CASTLE_SMALLROOM_REVERB);
    define_enum(mCastle, "SHORTPASSAGE", SEAL_CASTLE_SHORTPASSAGE_REVERB);
    define_enum(mCastle, "MEDIUMROOM", SEAL_CASTLE_MEDIUMROOM_REVERB);
    define_enum(mCastle, "LARGEROOM", SEAL_CASTLE_LARGEROOM_REVERB);
    define_enum(mCastle, "LONGPASSAGE", SEAL_CASTLE_LONGPASSAGE_REVERB);
    define_enum(mCastle, "HALL", SEAL_CASTLE_HALL_REVERB);
    define_enum(mCastle, "CUPBOARD", SEAL_CASTLE_CUPBOARD_REVERB);
    define_enum(mCastle, "COURTYARD", SEAL_CASTLE_COURTYARD_REVERB);
    define_enum(mCastle, "ALCOVE", SEAL_CASTLE_ALCOVE_REVERB);
    define_enum(mFactory, "SMALLROOM", SEAL_FACTORY_SMALLROOM_REVERB);
    define_enum(mFactory, "SHORTPASSAGE",
                    SEAL_FACTORY_SHORTPASSAGE_REVERB);
    define_enum(mFactory, "MEDIUMROOM", SEAL_FACTORY_MEDIUMROOM_REVERB);
    define_enum(mFactory, "LARGEROOM", SEAL_FACTORY_LARGEROOM_REVERB);
    define_enum(mFactory, "LONGPASSAGE", SEAL_FACTORY_LONGPASSAGE_REVERB);
    define_enum(mFactory, "HALL", SEAL_FACTORY_HALL_REVERB);
    define_enum(mFactory, "CUPBOARD", SEAL_FACTORY_CUPBOARD_REVERB);
    define_enum(mFactory, "COURTYARD", SEAL_FACTORY_COURTYARD_REVERB);
    define_enum(mFactory, "ALCOVE", SEAL_FACTORY_ALCOVE_REVERB);
    define_enum(mIcePalace, "SMALLROOM",
                    SEAL_ICEPALACE_SMALLROOM_REVERB);
    define_enum(mIcePalace, "SHORTPASSAGE",
                    SEAL_ICEPALACE_SHORTPASSAGE_REVERB);
    define_enum(mIcePalace, "MEDIUMROOM",
                    SEAL_ICEPALACE_MEDIUMROOM_REVERB);
    define_enum(mIcePalace, "LARGEROOM", SEAL_ICEPALACE_LARGEROOM_REVERB);
    define_enum(mIcePalace, "LONGPASSAGE",
                    SEAL_ICEPALACE_LONGPASSAGE_REVERB);
    define_enum(mIcePalace, "HALL", SEAL_ICEPALACE_HALL_REVERB);
    define_enum(mIcePalace, "CUPBOARD", SEAL_ICEPALACE_CUPBOARD_REVERB);
    define_enum(mIcePalace, "COURTYARD", SEAL_ICEPALACE_COURTYARD_REVERB);
    define_enum(mIcePalace, "ALCOVE", SEAL_ICEPALACE_ALCOVE_REVERB);
    define_enum(mSpaceStation, "SMALLROOM",
                    SEAL_SPACESTATION_SMALLROOM_REVERB);
    define_enum(mSpaceStation, "SHORTPASSAGE",
                    SEAL_SPACESTATION_SHORTPASSAGE_REVERB);
    define_enum(mSpaceStation, "MEDIUMROOM",
                    SEAL_SPACESTATION_MEDIUMROOM_REVERB);
    define_enum(mSpaceStation, "LARGEROOM",
                    SEAL_SPACESTATION_LARGEROOM_REVERB);
    define_enum(mSpaceStation, "LONGPASSAGE",
                    SEAL_SPACESTATION_LONGPASSAGE_REVERB);
    define_enum(mSpaceStation, "HALL", SEAL_SPACESTATION_HALL_REVERB);
    define_enum(mSpaceStation, "CUPBOARD",
                    SEAL_SPACESTATION_CUPBOARD_REVERB);
    define_enum(mSpaceStation, "ALCOVE", SEAL_SPACESTATION_ALCOVE_REVERB);
    define_enum(mWoodenGalleon, "SMALLROOM",
                    SEAL_WOODEN_SMALLROOM_REVERB);
    define_enum(mWoodenGalleon, "SHORTPASSAGE",
                    SEAL_WOODEN_SHORTPASSAGE_REVERB);
    define_enum(mWoodenGalleon, "MEDIUMROOM",
                    SEAL_WOODEN_MEDIUMROOM_REVERB);
    define_enum(mWoodenGalleon, "LARGEROOM",
                    SEAL_WOODEN_LARGEROOM_REVERB);
    define_enum(mWoodenGalleon, "LONGPASSAGE",
                    SEAL_WOODEN_LONGPASSAGE_REVERB);
    define_enum(mWoodenGalleon, "HALL", SEAL_WOODEN_HALL_REVERB);
    define_enum(mWoodenGalleon, "CUPBOARD", SEAL_WOODEN_CUPBOARD_REVERB);
    define_enum(mWoodenGalleon, "COURTYARD",
                    SEAL_WOODEN_COURTYARD_REVERB);
    define_enum(mWoodenGalleon, "ALCOVE", SEAL_WOODEN_ALCOVE_REVERB);
    define_enum(mSports, "EMPTYSTADIUM", SEAL_SPORT_EMPTYSTADIUM_REVERB);
    define_enum(mSports, "SQUASHCOURT", SEAL_SPORT_SQUASHCOURT_REVERB);
    define_enum(mSports, "SMALLSWIMMINGPOOL",
                    SEAL_SPORT_SMALLSWIMMINGPOOL_REVERB);
    define_enum(mSports, "LARGESWIMMINGPOOL",
                    SEAL_SPORT_LARGESWIMMINGPOOL_REVERB);
    define_enum(mSports, "GYMNASIUM", SEAL_SPORT_GYMNASIUM_REVERB);
    define_enum(mSports, "FULLSTADIUM", SEAL_SPORT_FULLSTADIUM_REVERB);
    define_enum(mSports, "STADIUMTANNOY",
                    SEAL_SPORT_STADIUMTANNOY_REVERB);
    define_enum(mPrefab, "WORKSHOP", SEAL_PREFAB_WORKSHOP_REVERB);
    define_enum(mPrefab, "SCHOOLROOM", SEAL_PREFAB_SCHOOLROOM_REVERB);
    define_enum(mPrefab, "PRACTISEROOM", SEAL_PREFAB_PRACTISEROOM_REVERB);
    define_enum(mPrefab, "OUTHOUSE", SEAL_PREFAB_OUTHOUSE_REVERB);
    define_enum(mPrefab, "CARAVAN", SEAL_PREFAB_CARAVAN_REVERB);
    define_enum(mDome, "TOMB", SEAL_DOME_TOMB_REVERB);
    define_enum(mDome, "SAINTPAULS", SEAL_DOME_SAINTPAULS_REVERB);
    define_enum(mPipe, "SMALL", SEAL_PIPE_SMALL_REVERB);
    define_enum(mPipe, "LONGTHIN", SEAL_PIPE_LONGTHIN_REVERB);
    define_enum(mPipe, "LARGE", SEAL_PIPE_LARGE_REVERB);
    define_enum(mPipe, "RESONANT", SEAL_PIPE_RESONANT_REVERB);
    define_enum(mOutdoors, "BACKYARD", SEAL_OUTDOORS_BACKYARD_REVERB);
    define_enum(mOutdoors, "ROLLINGPLAINS",
                    SEAL_OUTDOORS_ROLLINGPLAINS_REVERB);
    define_enum(mOutdoors, "DEEPCANYON", SEAL_OUTDOORS_DEEPCANYON_REVERB);
    define_enum(mOutdoors, "CREEK", SEAL_OUTDOORS_CREEK_REVERB);
    define_enum(mOutdoors, "VALLEY", SEAL_OUTDOORS_VALLEY_REVERB);
    define_enum(mMood, "HEAVEN", SEAL_MOOD_HEAVEN_REVERB);
    define_enum(mMood, "HELL", SEAL_MOOD_HELL_REVERB);
    define_enum(mMood, "MEMORY", SEAL_MOOD_MEMORY_REVERB);
    define_enum(mDriving, "COMMENTATOR", SEAL_DRIVING_COMMENTATOR_REVERB);
    define_enum(mDriving, "PITGARAGE", SEAL_DRIVING_PITGARAGE_REVERB);
    define_enum(mDriving, "INCAR_RACER", SEAL_DRIVING_INCAR_RACER_REVERB);
    define_enum(mDriving, "INCAR_SPORTS",
                    SEAL_DRIVING_INCAR_SPORTS_REVERB);
    define_enum(mDriving, "INCAR_LUXURY",
                    SEAL_DRIVING_INCAR_LUXURY_REVERB);
    define_enum(mDriving, "FULLGRANDSTAND",
                    SEAL_DRIVING_FULLGRANDSTAND_REVERB);
    define_enum(mDriving, "EMPTYGRANDSTAND",
                    SEAL_DRIVING_EMPTYGRANDSTAND_REVERB);
    define_enum(mDriving, "TUNNEL", SEAL_DRIVING_TUNNEL_REVERB);
    define_enum(mCity, "STREETS", SEAL_CITY_STREETS_REVERB);
    define_enum(mCity, "SUBWAY", SEAL_CITY_SUBWAY_REVERB);
    define_enum(mCity, "MUSEUM", SEAL_CITY_MUSEUM_REVERB);
    define_enum(mCity, "LIBRARY", SEAL_CITY_LIBRARY_REVERB);
    define_enum(mCity, "UNDERPASS", SEAL_CITY_UNDERPASS_REVERB);
    define_enum(mCity, "ABANDONED", SEAL_CITY_ABANDONED_REVERB);
    define_enum(mMisc, "DUSTYROOM", SEAL_DUSTYROOM_REVERB);
    define_enum(mMisc, "CHAPEL", SEAL_CHAPEL_REVERB);
    define_enum(mMisc, "SMALLWATERROOM", SEAL_SMALLWATERROOM_REVERB);
}

/*
 * Document-class:  Seal::EffectSlot
 *
 * EffectSlot is the container type for effects. A source can mix an effect in
 * an effect slot to filter the output sound.
 */
static void
bind_efs(void)
{
    VALUE cEffectSlot = rb_define_class_under(mSeal, "EffectSlot", rb_cObject);
    rb_define_alloc_func(cEffectSlot, alloc_efs);
    rb_define_method(cEffectSlot, "initialize", init_efs, -1);
    rb_define_method(cEffectSlot, "effect=", set_efs_effect, 1);
    rb_define_method(cEffectSlot, "effect", get_efs_effect, 0);
    rb_define_method(cEffectSlot, "feed", feed_efs, 2);
    rb_define_method(cEffectSlot, "gain=", set_efs_gain, 1);
    rb_define_method(cEffectSlot, "gain", get_efs_gain, 0);
    rb_define_method(cEffectSlot, "auto=", set_efs_auto, 1);
    rb_define_method(cEffectSlot, "auto", is_efs_auto, 0);
    rb_define_alias(cEffectSlot, "auto?", "auto");
}

/*
 * Document-class:  Seal::Listener
 *
 * Listener has a singleton instance representing the sole listener who hears
 * the sound.
 */
static void
bind_listener(void)
{
    VALUE cListener = rb_define_class_under(mSeal, "Listener", rb_cObject);
    VALUE listener = rb_data_object_alloc(cListener, 0, 0, 0);
    /* The singleton Listener instance. */
    rb_define_const(mSeal, "LISTENER", listener);
    rb_define_singleton_method(mSeal, "listener", get_listener, 0);
    singletonify(cListener);
    rb_define_method(cListener, "position=", set_listener_pos, 1);
    rb_define_method(cListener, "position", get_listener_pos, 0);
    rb_define_method(cListener, "velocity=", set_listener_vel, 1);
    rb_define_method(cListener, "velocity", get_listener_vel, 0);
    rb_define_method(cListener, "gain=", set_listener_gain, 1);
    rb_define_method(cListener, "gain", get_listener_gain, 0);
    rb_define_method(cListener, "orientation=", set_listener_orien, 1);
    rb_define_method(cListener, "orientation", get_listener_orien, 0);
}

/*
 * Document-module: Seal
 *
 * The top-level namespace of Seal.
 */
void
Init_seal(void)
{
    bind_core();
    bind_buf();
    bind_stream();
    bind_src();
    bind_rvb();
    bind_efs();
    bind_listener();
}
