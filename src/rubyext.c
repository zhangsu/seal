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
 */
static VALUE
cleanup()
{
    seal_cleanup();

    return Qnil;
}

/*
 *  call-seq:
 *      Seal::Buffer.allocate   -> buffer
 */
DEFINE_ALLOCATOR(buf)

/*
 *  call-seq:
 *      Seal::Buffer.new(filename [, format])   -> buffer
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
 *      Seal::Buffer.load(filename [, format])   -> buffer
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
 */
static VALUE
get_buf_size(VALUE rbuf)
{
    return get_obj_int(rbuf, seal_get_buf_size);
}

/*
 *  call-seq:
 *      buffer.frequency    -> fixnum
 */
static VALUE
get_buf_freq(VALUE rbuf)
{
    return get_obj_int(rbuf, seal_get_buf_freq);
}

/*
 *  call-seq:
 *      buffer.bit_depth    -> fixnum
 */
static VALUE
get_buf_bps(VALUE rbuf)
{
    return get_obj_int(rbuf, seal_get_buf_bps);
}

/*
 *  call-seq:
 *      buffer.channel_count    -> fixnum
 */
static VALUE
get_buf_nchannels(VALUE rbuf)
{
    return get_obj_int(rbuf, seal_get_buf_nchannels);
}

/*
 *  call-seq:
 *      Seal::Stream.allocate   -> stream
 */
DEFINE_ALLOCATOR(stream)

/*
 *  call-seq:
 *      Seal::Stream.new(filename [, format])   -> stream
 *      Seal::Stream.open(filename [, format])  -> stream
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
 */
static VALUE
get_stream_freq(VALUE rstream)
{
    return INT2NUM(extract_stream(rstream)->attr.freq);
}

/*
 *  call-seq:
 *      stream.bit_depth    -> fixnum
 */
static VALUE
get_stream_bps(VALUE rstream)
{
    return INT2NUM(extract_stream(rstream)->attr.bit_depth);
}

/*
 *  call-seq:
 *      stream.channel_count    -> fixnum
 */
static VALUE
get_stream_nchannels(VALUE rstream)
{
    return INT2NUM(extract_stream(rstream)->attr.nchannels);
}

/*
 *  call-seq:
 *      stream.rewind   -> stream
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
 */
static VALUE
close_stream(VALUE rstream)
{
    check_seal_err(seal_close_stream(DATA_PTR(rstream)));

    return rstream;
}

/*
 *  call-seq:
 *      Seal::Source.allocate -> source
 */
DEFINE_ALLOCATOR(src)

/*
 *  call-seq:
 *      Seal::Source.new  -> source
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
 */
static VALUE
play_src(VALUE rsrc)
{
    return src_op(rsrc, seal_play_src);
}

/*
 *  call-seq:
 *      source.pause -> source
 */
static VALUE
pause_src(VALUE rsrc)
{
    return src_op(rsrc, seal_pause_src);
}

/*
 *  call-seq:
 *      source.stop ->  source
 */
static VALUE
stop_src(VALUE rsrc)
{
    return src_op(rsrc, seal_stop_src);
}

/*
 *  call-seq:
 *      source.rewind ->    source
 */
static VALUE
rewind_src(VALUE rsrc)
{
    return src_op(rsrc, seal_rewind_src);
}

/*
 *  call-seq:
 *      source.detach -> source
 */
static VALUE
detach_src_audio(VALUE rsrc)
{
    return src_op(rsrc, seal_detach_src_audio);
}

/*
 *  call-seq:
 *      source.buffer = buffer  -> buffer
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
 */
static VALUE
get_src_buf(VALUE rsrc)
{
    return rb_iv_get(rsrc, "@buffer");
}

/*
 *  call-seq:
 *      source.stream = stream  -> stream
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
 */
static VALUE
get_src_stream(VALUE rsrc)
{
    return rb_iv_get(rsrc, "@stream");
}

/*
 *  call-seq:
 *      source.update   -> source
 */
static VALUE update_src(VALUE rsrc)
{
    return src_op(rsrc, seal_update_src);
}

/*
 *  call-seq:
 *      source.position = [flt, flt, flt]   -> [flt, flt, flt]
 */
static VALUE
set_src_pos(VALUE rsrc, VALUE value)
{
    return set_obj_3float(rsrc, value, seal_set_src_pos);
}

/*
 *  call-seq:
 *      source.position -> [flt, flt, flt]
 */
static VALUE
get_src_pos(VALUE rsrc)
{
    return get_obj_3float(rsrc, seal_get_src_pos);
}

/*
 *  call-seq:
 *      source.velocity = [flt, flt, flt]   -> [flt, flt, flt]
 */
static VALUE
set_src_vel(VALUE rsrc, VALUE value)
{
    return set_obj_3float(rsrc, value, seal_set_src_vel);
}

/*
 *  call-seq:
 *      source.velocity -> [flt, flt, flt]
 */
static VALUE
get_src_vel(VALUE rsrc)
{
    return get_obj_3float(rsrc, seal_get_src_vel);
}

/*
 *  call-seq:
 *      source.pitch = flt  -> [flt, flt, flt]
 */
static VALUE
set_src_pitch(VALUE rsrc, VALUE value)
{
    return set_obj_float(rsrc, value, seal_set_src_pitch);
}

/*
 *  call-seq:
 *      source.pitch    -> flt
 */
static VALUE
get_src_pitch(VALUE rsrc)
{
    return get_obj_float(rsrc, seal_get_src_pitch);
}

/*
 *  call-seq:
 *      source.gain = flt   -> [flt, flt, flt]
 */
static VALUE
set_src_gain(VALUE rsrc, VALUE value)
{
    return set_obj_float(rsrc, value, seal_set_src_gain);
}

/*
 *  call-seq:
 *      source.gain -> flt
 */
static VALUE
get_src_gain(VALUE rsrc)
{
    return get_obj_float(rsrc, seal_get_src_gain);
}

/*
 *  call-seq:
 *      source.auto = true or false -> true or false
 */
static VALUE
set_src_auto(VALUE rsrc, VALUE value)
{
    return set_obj_char(rsrc, value, seal_set_src_auto);
}

/*
 *  call-seq:
 *      source.auto     -> true or false
 */
static VALUE
is_src_auto(VALUE rsrc)
{
    return get_obj_char(rsrc, seal_is_src_auto);
}

/*
 *  call-seq:
 *      source.relative = true or false -> true or false
 */
static VALUE
set_src_relative(VALUE rsrc, VALUE value)
{
    return set_obj_char(rsrc, value, seal_set_src_relative);
}

/*
 *  call-seq:
 *      source.relative   -> true or false
 */
static VALUE
is_src_relative(VALUE rsrc)
{
    return get_obj_char(rsrc, seal_is_src_relative);
}

/*
 *  call-seq:
 *      source.looping = true or false  -> true or false
 */
static VALUE
set_src_looping(VALUE rsrc, VALUE value)
{
    return set_obj_char(rsrc, value, seal_set_src_looping);
}

/*
 *  call-seq:
 *      source.looping  -> true or false
 */
static VALUE
is_src_looping(VALUE rsrc)
{
    return get_obj_char(rsrc, seal_is_src_looping);
}

/*
 *  call-seq:
 *      source.queue_size = fixnum  -> true or false
 */
static VALUE
set_src_queue_size(VALUE rsrc, VALUE value)
{
    return set_obj_float(rsrc, value, seal_set_src_queue_size);
}

/*
 *  call-seq:
 *      source.queue_size   -> fixnum
 */
static VALUE
get_src_queue_size(VALUE rsrc)
{
    return get_obj_float(rsrc, seal_get_src_queue_size);
}

/*
 *  call-seq:
 *      source.chunk_size = fixnum  -> true or false
 */
static VALUE
set_src_chunk_size(VALUE rsrc, VALUE value)
{
    return set_obj_int(rsrc, value, seal_set_src_chunk_size);
}

/*
 *  call-seq:
 *      source.chunk_size   -> fixnum
 */
static VALUE
get_src_chunk_size(VALUE rsrc)
{
    return get_obj_int(rsrc, seal_get_src_chunk_size);
}
/*
 *  call-seq:
 *      source.type -> :streaming or :static
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
 *      slot.feed(index, slot)    -> slot
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
 *      Seal::Reverb.allocate -> reverb
 */
DEFINE_ALLOCATOR(rvb)

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
 *      Seal::Reverb.new  -> reverb
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
 */
static VALUE
set_src_density(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_density);
}

/*
 *  call-seq:
 *      reverb.density  -> flt
 */
static VALUE
get_rvb_density(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_density);
}

/*
 *  call-seq:
 *      reverb.diffusion = flt  -> flt
 */
static VALUE
set_rvb_diffusion(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_diffusion);
}

/*
 *  call-seq:
 *      reverb.diffusion  -> flt
 */
static VALUE
get_rvb_diffusion(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_diffusion);
}

/*
 *  call-seq:
 *      reverb.gain = flt  -> flt
 */
static VALUE
set_rvb_gain(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_gain);
}

/*
 *  call-seq:
 *      reverb.gain  -> flt
 */
static VALUE
get_rvb_gain(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_gain);
}

/*
 *  call-seq:
 *      reverb.hfgain = flt  -> flt
 */
static VALUE
set_rvb_hfgain(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_hfgain);
}

/*
 *  call-seq:
 *      reverb.hfgain  -> flt
 */
static VALUE
get_rvb_hfgain(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_hfgain);
}

/*
 *  call-seq:
 *      reverb.decay_time = flt  -> flt
 */
static VALUE
set_rvb_decay_time(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_decay_time);
}

/*
 *  call-seq:
 *      reverb.decay_time  -> flt
 */
static VALUE
get_rvb_decay_time(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_decay_time);
}

/*
 *  call-seq:
 *      reverb.hfdecay_ratio = flt  -> flt
 */
static VALUE
set_rvb_hfdecay_ratio(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_hfdecay_ratio);
}

/*
 *  call-seq:
 *      reverb.hfdecay_ratio  -> flt
 */
static VALUE
get_rvb_hfdecay_ratio(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_hfdecay_ratio);
}

/*
 *  call-seq:
 *      reverb.reflections_gain = flt  -> flt
 */
static VALUE
set_rvb_reflections_gain(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_reflections_gain);
}

/*
 *  call-seq:
 *      reverb.reflections_gain  -> flt
 */
static VALUE
get_rvb_reflections_gain(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_reflections_gain);
}

/*
 *  call-seq:
 *      reverb.reflections_delay = flt  -> flt
 */
static VALUE
set_rvb_reflections_delay(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_reflections_delay);
}

/*
 *  call-seq:
 *      reverb.reflections_delay  -> flt
 */
static VALUE
get_rvb_reflections_delay(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_reflections_delay);
}

/*
 *  call-seq:
 *      reverb.late_gain = flt  -> flt
 */
static VALUE
set_rvb_late_gain(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_late_gain);
}

/*
 *  call-seq:
 *      reverb.late_gain  -> flt
 */
static VALUE
get_rvb_late_gain(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_late_gain);
}

/*
 *  call-seq:
 *      reverb.late_delay = flt  -> flt
 */
static VALUE
set_rvb_late_delay(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_late_delay);
}

/*
 *  call-seq:
 *      reverb.late_delay  -> flt
 */
static VALUE
get_rvb_late_delay(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_late_delay);
}
/*
 *  call-seq:
 *      reverb.air_absorbtion_hfgain = flt  -> flt
 */
static VALUE
set_rvb_air_absorbtion_hfgain(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_air_absorbtion_hfgain);
}

/*
 *  call-seq:
 *      reverb.air_absorbtion_hfgain  -> flt
 */
static VALUE
get_rvb_air_absorbtion_hfgain(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_air_absorbtion_hfgain);
}

/*
 *  call-seq:
 *      reverb.room_rolloff_factor = flt  -> flt
 */
static VALUE
set_rvb_room_rolloff_factor(VALUE rrvb, VALUE value)
{
    return set_obj_float(rrvb, value, seal_set_rvb_room_rolloff_factor);
}

/*
 *  call-seq:
 *      reverb.room_rolloff_factor  -> flt
 */
static VALUE
get_rvb_room_rolloff_factor(VALUE rrvb)
{
    return get_obj_float(rrvb, seal_get_rvb_room_rolloff_factor);
}

/*
 *  call-seq:
 *      reverb.hfdecay_limited = true or false  -> true or false
 */
static VALUE
set_rvb_hfdecay_limited(VALUE rrvb, VALUE value)
{
    return set_obj_char(rrvb, value, seal_set_rvb_hfdecay_limited);
}

/*
 *  call-seq:
 *      reverb.hfdecay_limited  -> true or false
 */
static VALUE
is_rvb_hfdecay_limited(VALUE rrvb)
{
    return get_obj_char(rrvb, seal_is_rvb_hfdecay_limited);
}

/*
 *  call-seq:
 *      EffectSlot.allocate -> effect_slot
 */
DEFINE_ALLOCATOR(efs)

/*
 *  call-seq:
 *      effect_slot.effect = effect -> effect
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
 */
static VALUE
get_efs_effect(VALUE rslot)
{
    return rb_iv_get(rslot, "@effect");
}

/*
 *  call-seq:
 *      effect_slot.gain = flt  -> flt
 */
static VALUE
set_efs_gain(VALUE refs, VALUE value)
{
    return set_obj_float(refs, value, seal_set_efs_gain);
}

/*
 *  call-seq:
 *      effect_slot.gain    -> flt
 */
static VALUE
get_efs_gain(VALUE refs)
{
    return get_obj_float(refs, seal_get_efs_gain);
}

/*
 *  call-seq:
 *      effect_slot.auto = true or false    -> true or false
 */
static VALUE
set_efs_auto(VALUE refs, VALUE value)
{
    return set_obj_char(refs, value, seal_set_efs_auto);
}

/*
 *  call-seq:
 *      effect_slot.auto    -> true or false
 */
static VALUE
is_efs_auto(VALUE refs)
{
    return get_obj_char(refs, seal_is_efs_auto);
}

/*
 *  call-seq:
 *      Seal.listener  -> listener
 */
static VALUE
get_listener()
{
    return rb_const_get(mSeal, rb_intern("LISTENER"));
}

/*
 *  call-seq:
 *      Seal.listener.gain = flt   -> [flt, flt, flt]
 */
static VALUE
set_listener_gain(VALUE rlistener, VALUE value)
{
    return set_listener_float(value, seal_set_listener_gain);
}

/*
 *  call-seq:
 *      Seal.listener.gain -> flt
 */
static VALUE
get_listener_gain(VALUE rlistener, VALUE value)
{
    return get_listener_float(seal_get_listener_gain);
}

/*
 *  call-seq:
 *      Seal.listener.position = [flt, flt, flt]   -> [flt, flt, flt]
 */
static VALUE
set_listener_pos(VALUE rlistener, VALUE value)
{
    return set_listener_3float(value, seal_set_listener_pos);
}

/*
 *  call-seq:
 *      Seal.listener.position -> [flt, flt, flt]
 */
static VALUE
get_listener_pos(VALUE rlistener, VALUE value)
{
    return get_listener_3float(seal_get_listener_pos);
}

/*
 *  call-seq:
 *      Seal.listener.velocity = flt, flt, flt   -> [flt, flt, flt]
 */
static VALUE
set_listener_vel(VALUE rlistener, VALUE value)
{
    return set_listener_3float(value, seal_set_listener_vel);
}


/*
 *  call-seq:
 *      Seal.listener.velocity -> [flt, flt, flt]
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
 *  Examples:
 *      at, up = Seal.listener.orientation
 *      at    # => the `at' vector [flt, flt, flt]
 *      up    # => the `up' vector [flt, flt, flt]
 *      (at_x, at_y, ay_z), (up_x, up_y, up_z) = Seal.listener.orientation
 *      at_x  # => the x component of the `at' vector
 *      # ...
 *      up_z  # => the z component of the `up' vector
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

static void
bind_core(void)
{
    VALUE mFormat;
    mSeal = rb_define_module("Seal");
    eSealError = rb_define_class("SealError", rb_eException);
    rb_define_singleton_method(mSeal, "startup", startup, -1);
    rb_define_singleton_method(mSeal, "cleanup", cleanup, 0);
    mFormat = rb_define_module_under(mSeal, "Format");
    rb_define_const(mFormat, "WAV", name2sym(WAV_SYM));
    rb_define_const(mFormat, "OV", name2sym(OV_SYM));
    rb_define_const(mFormat, "MPG", name2sym(MPG_SYM));
}

static void
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

static void
bind_src_state(VALUE cSource)
{
    VALUE mState;

    mState = rb_define_module_under(cSource, "State");
    rb_define_const(mState, "INITIAL", name2sym(INITIAL_SYM));
    rb_define_const(mState, "PLAYING", name2sym(PLAYING_SYM));
    rb_define_const(mState, "PAUSED", name2sym(PAUSED_SYM));
    rb_define_const(mState, "STOPPED", name2sym(STOPPED_SYM));
}

static void
bind_src_type(VALUE cSource)
{
    VALUE mType;

    mType = rb_define_module_under(cSource, "Type");
    rb_define_const(mType, "UNDETERMINED", name2sym(UNDETERMINED_SYM));
    rb_define_const(mType, "STATIC", name2sym(STATIC_SYM));
    rb_define_const(mType, "STREAMING", name2sym(STREAMING_SYM));
}

static void
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
    bind_src_state(cSource);
    bind_src_type(cSource);
}

static void
bind_rvb_castle_presets(VALUE mPreset)
{
    VALUE mCastle = rb_define_module_under(mPreset, "Castle");
    define_enum(mCastle, "SMALLROOM", SEAL_CASTLE_SMALLROOM_REVERB);
    define_enum(mCastle, "SHORTPASSAGE", SEAL_CASTLE_SHORTPASSAGE_REVERB);
    define_enum(mCastle, "MEDIUMROOM", SEAL_CASTLE_MEDIUMROOM_REVERB);
    define_enum(mCastle, "LARGEROOM", SEAL_CASTLE_LARGEROOM_REVERB);
    define_enum(mCastle, "LONGPASSAGE", SEAL_CASTLE_LONGPASSAGE_REVERB);
    define_enum(mCastle, "HALL", SEAL_CASTLE_HALL_REVERB);
    define_enum(mCastle, "CUPBOARD", SEAL_CASTLE_CUPBOARD_REVERB);
    define_enum(mCastle, "COURTYARD", SEAL_CASTLE_COURTYARD_REVERB);
    define_enum(mCastle, "ALCOVE", SEAL_CASTLE_ALCOVE_REVERB);
}

static void
bind_rvb_factory_presets(VALUE mPreset)
{
    VALUE mFactory = rb_define_module_under(mPreset, "Factory");
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
}

static void
bind_rvb_ice_palace_presets(VALUE mPreset)
{
    VALUE mIcePalace = rb_define_module_under(mPreset, "IcePalace");
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
}

static void
bind_rvb_space_station_presets(VALUE mPreset)
{
    VALUE mSpaceStation = rb_define_module_under(mPreset, "SpaceStation");
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
}

static void
bind_rvb_wooden_galleon_presets(VALUE mPreset)
{
    VALUE mWoodenGalleon = rb_define_module_under(mPreset, "WoodenGalleon");
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
}

static void
bind_rvb_sports_presets(VALUE mPreset)
{
    VALUE mSports = rb_define_module_under(mPreset, "Sports");
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
}

static void
bind_rvb_prefab_presets(VALUE mPreset)
{
    VALUE mPrefab = rb_define_module_under(mPreset, "Prefab");
    define_enum(mPrefab, "WORKSHOP", SEAL_PREFAB_WORKSHOP_REVERB);
    define_enum(mPrefab, "SCHOOLROOM", SEAL_PREFAB_SCHOOLROOM_REVERB);
    define_enum(mPrefab, "PRACTISEROOM", SEAL_PREFAB_PRACTISEROOM_REVERB);
    define_enum(mPrefab, "OUTHOUSE", SEAL_PREFAB_OUTHOUSE_REVERB);
    define_enum(mPrefab, "CARAVAN", SEAL_PREFAB_CARAVAN_REVERB);
}

static void
bind_rvb_dome_presets(VALUE mPreset)
{
    VALUE mDome = rb_define_module_under(mPreset, "Dome");
    define_enum(mDome, "TOMB", SEAL_DOME_TOMB_REVERB);
    define_enum(mDome, "SAINTPAULS", SEAL_DOME_SAINTPAULS_REVERB);
}

static void
bind_rvb_pipe_presets(VALUE mPreset)
{
    VALUE mPipe = rb_define_module_under(mPreset, "Pipe");
    define_enum(mPipe, "SMALL", SEAL_PIPE_SMALL_REVERB);
    define_enum(mPipe, "LONGTHIN", SEAL_PIPE_LONGTHIN_REVERB);
    define_enum(mPipe, "LARGE", SEAL_PIPE_LARGE_REVERB);
    define_enum(mPipe, "RESONANT", SEAL_PIPE_RESONANT_REVERB);
}

static void
bind_rvb_outdoors_presets(VALUE mPreset)
{
    VALUE mOutdoors = rb_define_module_under(mPreset, "Outdoors");
    define_enum(mOutdoors, "BACKYARD", SEAL_OUTDOORS_BACKYARD_REVERB);
    define_enum(mOutdoors, "ROLLINGPLAINS",
                    SEAL_OUTDOORS_ROLLINGPLAINS_REVERB);
    define_enum(mOutdoors, "DEEPCANYON", SEAL_OUTDOORS_DEEPCANYON_REVERB);
    define_enum(mOutdoors, "CREEK", SEAL_OUTDOORS_CREEK_REVERB);
    define_enum(mOutdoors, "VALLEY", SEAL_OUTDOORS_VALLEY_REVERB);
}

static void
bind_rvb_mood_presets(VALUE mPreset)
{
    VALUE mMood = rb_define_module_under(mPreset, "Mood");
    define_enum(mMood, "HEAVEN", SEAL_MOOD_HEAVEN_REVERB);
    define_enum(mMood, "HELL", SEAL_MOOD_HELL_REVERB);
    define_enum(mMood, "MEMORY", SEAL_MOOD_MEMORY_REVERB);
}

static void
bind_rvb_driving_presets(VALUE mPreset)
{
    VALUE mDriving = rb_define_module_under(mPreset, "Driving");
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
}

static void
bind_rvb_city_presets(VALUE mPreset)
{
    VALUE mCity = rb_define_module_under(mPreset, "City");
    define_enum(mCity, "STREETS", SEAL_CITY_STREETS_REVERB);
    define_enum(mCity, "SUBWAY", SEAL_CITY_SUBWAY_REVERB);
    define_enum(mCity, "MUSEUM", SEAL_CITY_MUSEUM_REVERB);
    define_enum(mCity, "LIBRARY", SEAL_CITY_LIBRARY_REVERB);
    define_enum(mCity, "UNDERPASS", SEAL_CITY_UNDERPASS_REVERB);
    define_enum(mCity, "ABANDONED", SEAL_CITY_ABANDONED_REVERB);
}

static void
bind_rvb_misc_presets(VALUE mPreset)
{
    VALUE mMisc = rb_define_module_under(mPreset, "Misc");
    define_enum(mMisc, "DUSTYROOM", SEAL_DUSTYROOM_REVERB);
    define_enum(mMisc, "CHAPEL", SEAL_CHAPEL_REVERB);
    define_enum(mMisc, "SMALLWATERROOM", SEAL_SMALLWATERROOM_REVERB);
}


static void
bind_rvb_presets(VALUE cReverb)
{
    VALUE mPreset = rb_define_module_under(cReverb, "Preset");
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
    bind_rvb_castle_presets(mPreset);
    bind_rvb_factory_presets(mPreset);
    bind_rvb_ice_palace_presets(mPreset);
    bind_rvb_space_station_presets(mPreset);
    bind_rvb_wooden_galleon_presets(mPreset);
    bind_rvb_sports_presets(mPreset);
    bind_rvb_prefab_presets(mPreset);
    bind_rvb_dome_presets(mPreset);
    bind_rvb_pipe_presets(mPreset);
    bind_rvb_outdoors_presets(mPreset);
    bind_rvb_mood_presets(mPreset);
    bind_rvb_driving_presets(mPreset);
    bind_rvb_city_presets(mPreset);
    bind_rvb_misc_presets(mPreset);
}

static void
bind_rvb(void)
{
    VALUE cReverb = rb_define_class_under(mSeal, "Reverb", rb_cObject);
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
    bind_rvb_presets(cReverb);
}

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

static void
bind_listener(void)
{
    VALUE cListener = rb_define_class_under(mSeal, "Listener", rb_cObject);
    VALUE listener = rb_data_object_alloc(cListener, 0, 0, 0);
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

/* CRuby extension entry point. */
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
