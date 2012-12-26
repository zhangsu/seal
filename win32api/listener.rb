require File.join(File.dirname(__FILE__), 'core')

module Seal
  class Listener
    include Helper

    SET_GAIN = SealAPI.new('set_listener_gain', 'i')
    GET_GAIN = SealAPI.new('get_listener_gain', 'p')
    SET_POS = SealAPI.new('set_listener_pos', 'iii')
    GET_POS = SealAPI.new('get_listener_pos', 'ppp')
    SET_VEL = SealAPI.new('set_listener_vel', 'iii')
    GET_VEL = SealAPI.new('get_listener_vel', 'ppp')
    SET_ORIEN = SealAPI.new('set_listener_orien', 'p')
    GET_ORIEN = SealAPI.new('get_listener_orien', 'p')

    def gain=(gain)
      check_error(SET_GAIN[[gain].pack('f').unpack('i')[0]])
      gain
    end

    def gain
      gain_buffer = '    '
      check_error(GET_GAIN[gain_buffer])
      return gain_buffer.unpack('f')[0]
    end

    def position=(position)
      set_3float(position, SET_POS)
    end

    def position
      get_3float(GET_POS)
    end

    def velocity=(velocity)
      set_3float(velocity, SET_VEL)
    end

    def velocity
      get_3float(GET_VEL)
    end

    def orientation=(orientation)
      check_error(SET_ORIEN[orientation.flatten.pack('f*')])
    end

    def orientation
      orientation_buffer = '    ' * 6
      check_error(GET_ORIEN[orientation_buffer])
      orientation = orientation_buffer.unpack('f*')
      [orientation[0..2], orientation[3..5]]
    end

  private
    def set_3float(float_tuple, setter)
      integer_tuple = float_tuple.pack('f*').unpack('i*')
      check_error(setter[*integer_tuple])
      return float_tuple
    end

    def get_3float(getter)
      float_tuple_buffers = Array.new(3) { '    ' }
      check_error(getter[*float_tuple_buffers])
      return float_tuple_buffers.join.unpack('f*')
    end
  end

  LISTENER = Listener.new

  def self.listener
    LISTENER
  end

  class << Listener
    undef allocate
    undef new
  end
end