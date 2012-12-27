require File.join(File.dirname(__FILE__), 'core')

module Seal
  class Source
    include Helper

    INIT = SealAPI.new('init_src', 'p')
    DESTROY = SealAPI.new('destroy_src', 'p')
    PLAY = SealAPI.new('play_src', 'p')
    STOP = SealAPI.new('stop_src', 'p')
    REWIND = SealAPI.new('rewind_src', 'p')
    PAUSE = SealAPI.new('pause_src', 'p')
    DETACH = SealAPI.new('detach_src_audio', 'p')
    SET_BUF = SealAPI.new('set_src_buf', 'pp')
    SET_STREAM = SealAPI.new('set_src_stream', 'pp')
    FEED_EFS = SealAPI.new('feed_efs', 'ppi')
    UPDATE = SealAPI.new('update_src', 'p')
    SET_POS = SealAPI.new('set_src_pos', 'piii')
    SET_VEL = SealAPI.new('set_src_vel', 'piii')
    SET_GAIN = SealAPI.new('set_src_gain', 'pi')
    SET_PITCH = SealAPI.new('set_src_pitch', 'pi')
    SET_AUTO = SealAPI.new('set_src_auto', 'pi')
    SET_RELATIVE = SealAPI.new('set_src_relative', 'pi')
    SET_LOOPING = SealAPI.new('set_src_looping', 'pi')
    SET_QUEUE_SIZE = SealAPI.new('set_src_queue_size', 'pi')
    SET_CHUNK_SIZE = SealAPI.new('set_src_chunk_size', 'pi')
    GET_POS = SealAPI.new('get_src_pos', 'pppp')
    GET_VEL = SealAPI.new('get_src_vel', 'pppp')
    GET_GAIN = SealAPI.new('get_src_gain', 'pp')
    GET_PITCH = SealAPI.new('get_src_pitch', 'pp')
    GET_AUTO = SealAPI.new('is_src_auto', 'pp')
    GET_RELATIVE = SealAPI.new('is_src_relative', 'pp')
    GET_LOOPING = SealAPI.new('is_src_looping', 'pp')
    GET_QUEUE_SIZE = SealAPI.new('get_src_queue_size', 'pp')
    GET_CHUNK_SIZE = SealAPI.new('get_src_chunk_size', 'pp')
    GET_TYPE = SealAPI.new('get_src_type', 'pp')
    GET_STATE = SealAPI.new('get_src_state', 'pp')

    def initialize
      @source = '    ' * 5
      check_error(INIT[@source])
      ObjectSpace.define_finalizer(self, Helper.free(@source, DESTROY))
      self
    end

    def play
      operate(PLAY)
    end

    def stop
      operate(STOP)
    end

    def rewind
      operate(REWIND)
    end

    def pause
      operate(PAUSE)
    end

    def buffer=(buffer)
      set_audio(:@buffer, buffer, SET_BUF)
    end

    def stream=(stream)
      set_audio(:@stream, stream, SET_STREAM)
    end

    attr_reader :buffer, :stream

    def feed(effect_slot, index)
      native_efs_obj = effect_slot.instance_variable_get(:@effect_slot)
      check_error(FEED_EFS[@source, native_efs_obj, index])
      self
    end

    def update
      operate(UPDATE)
    end

    def position=(position)
      set_3float(position, SET_POS)
    end

    def velocity=(velocity)
      set_3float(velocity, SET_VEL)
    end

    def gain=(gain)
      set_obj_float(@source, gain, SET_GAIN)
    end

    def pitch=(pitch)
      set_obj_float(@source, pitch, SET_PITCH)
    end

    def auto=(auto)
      set_obj_char(@source, auto, SET_AUTO)
    end

    def queue_size=(queue_size)
      set_obj_int(@source, queue_size, SET_QUEUE_SIZE)
    end

    def chunk_size=(chunk_size)
      set_obj_int(@source, chunk_size, SET_CHUNK_SIZE)
    end

    def relative=(relative)
      set_obj_char(@source, relative, SET_RELATIVE)
    end

    def looping=(looping)
      set_obj_char(@source, looping, SET_LOOPING)
    end

    def position
      get_3float(GET_POS)
    end

    def velocity
      get_3float(GET_VEL)
    end

    def gain
      get_obj_float(@source, GET_GAIN)
    end

    def pitch
      get_obj_float(@source, GET_PITCH)
    end

    def auto
      get_obj_char(@source, GET_AUTO)
    end

    def relative
      get_obj_char(@source, GET_RELATIVE)
    end

    def looping
      get_obj_char(@source, GET_LOOPING)
    end

    alias auto? auto
    alias relative? relative
    alias looping? looping

    def queue_size
      get_obj_int(@source, GET_QUEUE_SIZE)
    end

    def chunk_size
      get_obj_int(@source, GET_CHUNK_SIZE)
    end

    def type
      case get_obj_int(@source, GET_TYPE)
      when Type::STATIC
        Type::STATIC
      when Type::STREAMING
        Type::STREAMING
      else
        Type::UNDETERMINED
      end
    end

    def state
      case get_obj_int(@source, GET_STATE)
      when State::PLAYING
        State::PLAYING
      when State::PAUSED
        State::PAUSED
      when State::STOPPED
        State::STOPPED
      else
        State::INITIAL
      end
    end

  private
    def operate(operation)
      check_error(operation[@source])
      self
    end

    def set_audio(var, audio, setter)
      if audio.nil?
        operate(DETACH)
      else
        check_error(setter[@source, audio.instance_variable_get(var)])
      end
      instance_variable_set(var, audio)
      audio
    end

    def set_3float(float_tuple, setter)
      integer_tuple = float_tuple.pack('f*').unpack('i*')
      check_error(setter[@source, *integer_tuple])
      float_tuple
    end

    def get_3float(getter)
      float_tuple_buffers = Array.new(3) { '    ' }
      check_error(getter[@source, *float_tuple_buffers])
      float_tuple_buffers.join.unpack('f*')
    end

    module State
      Helper.define_enum(self, [
        :INITIAL,
        :PLAYING,
        :PAUSED,
        :STOPPED
      ])
    end

    module Type
      Helper.define_enum(self, [
        :UNDETERMINED,
        :STATIC,
        :STREAMING
      ])
    end
  end
end