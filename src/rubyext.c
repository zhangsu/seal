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

#define DEFINE_SETTER(type, obj, attr)                                      \
static VALUE                                                                \
set_##obj##_##attr(VALUE r##obj, VALUE value)                               \
{                                                                           \
    return set##_obj_##type(r##obj, value, seal_set_##obj##_##attr);        \
}

#define DEFINE_GETTER(type, obj, attr)                                      \
static VALUE                                                                \
get_##obj##_##attr(VALUE r##obj)                                            \
{                                                                           \
    return get##_obj_##type(r##obj, seal_get_##obj##_##attr);               \
}

#define DEFINE_PREDICATE(obj, attr)                                         \
static VALUE                                                                \
is_##obj##_##attr(VALUE r##obj)                                             \
{                                                                           \
    return get_obj_char(r##obj, seal_is_##obj##_##attr);                    \
}

#define DEFINE_LISTENER_SETTER(type, attr)                                  \
static VALUE                                                                \
set_listener_##attr(VALUE rlistener, VALUE value)                           \
{                                                                           \
    return set_listener_##type(value, seal_set_listener_##attr);            \
}

#define DEFINE_LISTENER_GETTER(type, attr)                                  \
static VALUE                                                                \
get_listener_##attr(VALUE rlistener)                                        \
{                                                                           \
    return get_listener_##type(seal_get_listener_##attr);                   \
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
DEFINE_DEALLOCATOR(reverb)
DEFINE_DEALLOCATOR(effect_slot)

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

static seal_fmt_t
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
DEFINE_GETTER(int, buf, size)

/*
 *  call-seq:
 *      buffer.frequency    -> fixnum
 */
DEFINE_GETTER(int, buf, freq)

/*
 *  call-seq:
 *      buffer.bit_depth    -> fixnum
 */
DEFINE_GETTER(int, buf, bps)

/*
 *  call-seq:
 *      buffer.channel_count    -> fixnum
 */
DEFINE_GETTER(int, buf, nchannels)

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
DEFINE_SETTER(3float, src, pos)

/*
 *  call-seq:
 *      source.position -> [flt, flt, flt]
 */
DEFINE_GETTER(3float, src, pos)

/*
 *  call-seq:
 *      source.velocity = [flt, flt, flt]   -> [flt, flt, flt]
 */
DEFINE_SETTER(3float, src, vel)

/*
 *  call-seq:
 *      source.velocity -> [flt, flt, flt]
 */
DEFINE_GETTER(3float, src, vel)

/*
 *  call-seq:
 *      source.pitch = flt  -> [flt, flt, flt]
 */
DEFINE_SETTER(float, src, pitch)

/*
 *  call-seq:
 *      source.pitch    -> flt
 */
DEFINE_GETTER(float, src, pitch)

/*
 *  call-seq:
 *      source.gain = flt   -> [flt, flt, flt]
 */
DEFINE_SETTER(float, src, gain)

/*
 *  call-seq:
 *      source.gain -> flt
 */
DEFINE_GETTER(float, src, gain)

/*
 *  call-seq:
 *      source.auto = true or false -> true or false
 */
DEFINE_SETTER(char, src, auto)

/*
 *  call-seq:
 *      source.auto     -> true or false
 *      source.auto?    -> true or false
 */
DEFINE_PREDICATE(src, auto)

/*
 *  call-seq:
 *      source.relative = true or false -> true or false
 */
DEFINE_SETTER(char, src, relative)

/*
 *  call-seq:
 *      source.relative   -> true or false
 *      source.relative?  -> true or false
 */
DEFINE_PREDICATE(src, relative)

/*
 *  call-seq:
 *      source.looping = true or false  -> true or false
 */
DEFINE_SETTER(char, src, looping)

/*
 *  call-seq:
 *      source.looping  -> true or false
 *      source.looping? -> true or false
 */
DEFINE_PREDICATE(src, looping)

/*
 *  call-seq:
 *      source.queue_size = fixnum  -> true or false
 */
DEFINE_SETTER(float, src, queue_size)

/*
 *  call-seq:
 *      source.queue_size   -> fixnum
 */
DEFINE_GETTER(float, src, queue_size)

/*
 *  call-seq:
 *      source.chunk_size = fixnum  -> true or false
 */
DEFINE_SETTER(int, src, chunk_size)

/*
 *  call-seq:
 *      source.chunk_size   -> fixnum
 */
DEFINE_GETTER(int, src, chunk_size)
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
static VALUE
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
 *      Seal::Reverb.allocate -> reverb
 */
DEFINE_ALLOCATOR(reverb)

/*
 *  call-seq:
 *      Seal::Reverb.new  -> reverb
 */
static VALUE
init_reverb(VALUE rreverb)
{
    check_seal_err(seal_init_reverb(DATA_PTR(rreverb)));

    return rreverb;
}

/*
 *  call-seq:
 *      reverb.density = flt  -> flt
 */
DEFINE_SETTER(float, reverb, density)

/*
 *  call-seq:
 *      reverb.density  -> flt
 */
DEFINE_GETTER(float, reverb, density)

/*
 *  call-seq:
 *      reverb.diffusion = flt  -> flt
 */
DEFINE_SETTER(float, reverb, diffusion)

/*
 *  call-seq:
 *      reverb.diffusion  -> flt
 */
DEFINE_GETTER(float, reverb, diffusion)

/*
 *  call-seq:
 *      reverb.gain = flt  -> flt
 */
DEFINE_SETTER(float, reverb, gain)

/*
 *  call-seq:
 *      reverb.gain  -> flt
 */
DEFINE_GETTER(float, reverb, gain)

/*
 *  call-seq:
 *      reverb.hfgain = flt  -> flt
 */
DEFINE_SETTER(float, reverb, hfgain)

/*
 *  call-seq:
 *      reverb.hfgain  -> flt
 */
DEFINE_GETTER(float, reverb, hfgain)

/*
 *  call-seq:
 *      reverb.decay_time = flt  -> flt
 */
DEFINE_SETTER(float, reverb, decay_time)

/*
 *  call-seq:
 *      reverb.decay_time  -> flt
 */
DEFINE_GETTER(float, reverb, decay_time)

/*
 *  call-seq:
 *      reverb.hfdecay_ratio = flt  -> flt
 */
DEFINE_SETTER(float, reverb, hfdecay_ratio)

/*
 *  call-seq:
 *      reverb.hfdecay_ratio  -> flt
 */
DEFINE_GETTER(float, reverb, hfdecay_ratio)

/*
 *  call-seq:
 *      reverb.reflections_gain = flt  -> flt
 */
DEFINE_SETTER(float, reverb, reflections_gain)

/*
 *  call-seq:
 *      reverb.reflections_gain  -> flt
 */
DEFINE_GETTER(float, reverb, reflections_gain)

/*
 *  call-seq:
 *      reverb.reflections_delay = flt  -> flt
 */
DEFINE_SETTER(float, reverb, reflections_delay)

/*
 *  call-seq:
 *      reverb.reflections_delay  -> flt
 */
DEFINE_GETTER(float, reverb, reflections_delay)

/*
 *  call-seq:
 *      reverb.late_gain = flt  -> flt
 */
DEFINE_SETTER(float, reverb, late_gain)

/*
 *  call-seq:
 *      reverb.late_gain  -> flt
 */
DEFINE_GETTER(float, reverb, late_gain)

/*
 *  call-seq:
 *      reverb.late_delay = flt  -> flt
 */
DEFINE_SETTER(float, reverb, late_delay)

/*
 *  call-seq:
 *      reverb.late_delay  -> flt
 */
DEFINE_GETTER(float, reverb, late_delay)
/*
 *  call-seq:
 *      reverb.air_absorbtion_hfgain = flt  -> flt
 */
DEFINE_SETTER(float, reverb, air_absorbtion_hfgain)

/*
 *  call-seq:
 *      reverb.air_absorbtion_hfgain  -> flt
 */
DEFINE_GETTER(float, reverb, air_absorbtion_hfgain)

/*
 *  call-seq:
 *      reverb.room_rolloff_factor = flt  -> flt
 */
DEFINE_SETTER(float, reverb, room_rolloff_factor)

/*
 *  call-seq:
 *      reverb.room_rolloff_factor  -> flt
 */
DEFINE_GETTER(float, reverb, room_rolloff_factor)

/*
 *  call-seq:
 *      reverb.hfdecay_limited = true or false  -> true or false
 */
DEFINE_SETTER(char, reverb, hfdecay_limited)

/*
 *  call-seq:
 *      reverb.hfdecay_limited  -> true or false
 *      reverb.hfdecay_limited? -> true or false
 */
DEFINE_PREDICATE(reverb, hfdecay_limited)

/*
 *  call-seq:
 *      EffectSlot.allocate -> effect_slot
 */
DEFINE_ALLOCATOR(effect_slot)

/*
 *  call-seq:
 *      effect_slot.effect = effect -> effect
 */
static VALUE
set_effect_slot_effect(VALUE rslot, VALUE reffect)
{
    void* effect;
    seal_err_t err;

    if (NIL_P(reffect)) {
        err = seal_set_effect_slot_effect(DATA_PTR(rslot), 0);
    } else {
        Data_Get_Struct(reffect, void*, effect);
        err = seal_set_effect_slot_effect(DATA_PTR(rslot), effect);
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
init_effect_slot(int argc, VALUE* argv, VALUE rslot)
{
    VALUE reffect;

    rb_scan_args(argc, argv, "01", &reffect);
    check_seal_err(seal_init_effect_slot(DATA_PTR(rslot)));
    if (!NIL_P(reffect))
        set_effect_slot_effect(rslot, reffect);

    return rslot;
}

/*
 *  call-seq:
 *      effect_slot.effect  -> effect
 */
static VALUE
get_effect_slot_effect(VALUE rslot)
{
    return rb_iv_get(rslot, "@effect");
}

/*
 *  call-seq:
 *      effect_slot.gain = flt  -> flt
 */
DEFINE_SETTER(float, effect_slot, gain)

/*
 *  call-seq:
 *      effect_slot.gain    -> flt
 */
DEFINE_GETTER(float, effect_slot, gain)

/*
 *  call-seq:
 *      effect_slot.auto = true or false    -> true or false
 */
DEFINE_SETTER(char, effect_slot, auto)

/*
 *  call-seq:
 *      effect_slot.auto    -> true or false
 *      effect_slot.auto?   -> true or false
 */
DEFINE_PREDICATE(effect_slot, auto)

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
DEFINE_LISTENER_SETTER(float, gain)

/*
 *  call-seq:
 *      Seal.listener.gain -> flt
 */
DEFINE_LISTENER_GETTER(float, gain)

/*
 *  call-seq:
 *      Seal.listener.position = [flt, flt, flt]   -> [flt, flt, flt]
 */
DEFINE_LISTENER_SETTER(3float, pos)

/*
 *  call-seq:
 *      Seal.listener.position -> [flt, flt, flt]
 */
DEFINE_LISTENER_GETTER(3float, pos)

/*
 *  call-seq:
 *      Seal.listener.velocity = flt, flt, flt   -> [flt, flt, flt]
 */
DEFINE_LISTENER_SETTER(3float, vel)


/*
 *  call-seq:
 *      Seal.listener.velocity -> [flt, flt, flt]
 */
DEFINE_LISTENER_GETTER(3float, vel)

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
bind_core(void)
{
    VALUE mFormat;
    mSeal = rb_define_module("Seal");
    eSealError = rb_define_class("SealError", rb_eException);
    rb_define_singleton_method(mSeal, "startup", startup, -1);
    rb_define_singleton_method(mSeal, "cleanup", cleanup, 0);
    mFormat = rb_define_module_under(mSeal, "Format");
    rb_define_const(mFormat, "WAV", name2sym(WAV_SYMBOL));
    rb_define_const(mFormat, "OV", name2sym(OV_SYMBOL));
    rb_define_const(mFormat, "MPG", name2sym(MPG_SYMBOL));
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
bind_src(void)
{
    VALUE mState;
    VALUE mType;
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
    rb_define_const(mState, "INITIAL", name2sym(INITIAL_SYMBOL));
    rb_define_const(mState, "PLAYING", name2sym(PLAYING_SYMBOL));
    rb_define_const(mState, "PAUSED", name2sym(PAUSED_SYMBOL));
    rb_define_const(mState, "STOPPED", name2sym(STOPPED_SYMBOL));
    mType = rb_define_module_under(cSource, "Type");
    rb_define_const(mType, "UNDETERMINED", name2sym(UNDETERMINED_SYMBOL));
    rb_define_const(mType, "STATIC", name2sym(STATIC_SYMBOL));
    rb_define_const(mType, "STREAMING", name2sym(STREAMING_SYMBOL));
}

static void
bind_reverb(void)
{
    VALUE cReverb = rb_define_class_under(mSeal, "Reverb", rb_cObject);
    rb_define_alloc_func(cReverb, alloc_reverb);
    rb_define_method(cReverb, "initialize", init_reverb, 0);
    rb_define_method(cReverb, "density=", set_reverb_diffusion, 1);
    rb_define_method(cReverb, "density", get_reverb_diffusion, 0);
    rb_define_method(cReverb, "diffusion=", set_reverb_diffusion, 1);
    rb_define_method(cReverb, "diffusion", get_reverb_diffusion, 0);
    rb_define_method(cReverb, "gain=", set_reverb_gain, 1);
    rb_define_method(cReverb, "gain", get_reverb_gain, 0);
    rb_define_method(cReverb, "hfgain=", set_reverb_hfgain, 1);
    rb_define_method(cReverb, "hfgain", get_reverb_hfgain, 0);
    rb_define_method(cReverb, "decay_time=", set_reverb_decay_time, 1);
    rb_define_method(cReverb, "decay_time", get_reverb_decay_time, 0);
    rb_define_method(cReverb, "hfdecay_ratio=", set_reverb_hfdecay_ratio, 1);
    rb_define_method(cReverb, "hfdecay_ratio", get_reverb_hfdecay_ratio, 0);
    rb_define_method(cReverb, "reflections_gain=",
                     set_reverb_reflections_gain, 1);
    rb_define_method(cReverb, "reflections_gain",
                     get_reverb_reflections_gain, 0);
    rb_define_method(cReverb, "reflections_delay=",
                     set_reverb_reflections_delay, 1);
    rb_define_method(cReverb, "reflections_delay",
                     get_reverb_reflections_delay, 0);
    rb_define_method(cReverb, "late_gain=", set_reverb_late_gain, 1);
    rb_define_method(cReverb, "late_gain", get_reverb_late_gain, 0);
    rb_define_method(cReverb, "late_delay=", set_reverb_late_delay, 1);
    rb_define_method(cReverb, "late_delay", get_reverb_late_delay, 0);
    rb_define_method(cReverb, "air_absorbtion_hfgain=",
                     set_reverb_air_absorbtion_hfgain, 1);
    rb_define_method(cReverb, "air_absorbtion_hfgain",
                     get_reverb_air_absorbtion_hfgain, 0);
    rb_define_method(cReverb, "room_rolloff_factor=",
                     set_reverb_room_rolloff_factor, 1);
    rb_define_method(cReverb, "room_rolloff_factor",
                     get_reverb_room_rolloff_factor, 0);
    rb_define_method(cReverb, "hfdecay_limited=",
                     set_reverb_hfdecay_limited, 1);
    rb_define_method(cReverb, "hfdecay_limited",
                     is_reverb_hfdecay_limited, 0);
    rb_define_alias(cReverb, "hfdecay_limited?", "hfdecay_limited");
}

static void
bind_effect_slot(void)
{
    VALUE cEffectSlot = rb_define_class_under(mSeal, "EffectSlot", rb_cObject);
    rb_define_alloc_func(cEffectSlot, alloc_effect_slot);
    rb_define_method(cEffectSlot, "initialize", init_effect_slot, -1);
    rb_define_method(cEffectSlot, "effect=", set_effect_slot_effect, 1);
    rb_define_method(cEffectSlot, "effect", get_effect_slot_effect, 0);
    rb_define_method(cEffectSlot, "gain=", set_effect_slot_gain, 1);
    rb_define_method(cEffectSlot, "gain", get_effect_slot_gain, 0);
    rb_define_method(cEffectSlot, "auto=", set_effect_slot_auto, 1);
    rb_define_method(cEffectSlot, "auto", is_effect_slot_auto, 0);
    rb_define_alias(cEffectSlot, "auto?", "auto");
}

static void
bind_listener(void)
{
    VALUE cListener = rb_define_class_under(mSeal, "Listener", rb_cObject);
    VALUE listener = rb_data_object_alloc(cListener, 0, 0, 0);
    rb_define_const(mSeal, "LISTENER", listener);
    rb_define_singleton_method(mSeal, "listener", get_listener, 0);
    rb_undef_alloc_func(cListener);
    rb_undef_method(rb_singleton_class(cListener), "new");
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
    bind_reverb();
    bind_effect_slot();
    bind_listener();
}
