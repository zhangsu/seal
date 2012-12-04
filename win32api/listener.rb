require './core'

module Seal
  class Listener
    SET_GAIN = SealAPI.new('set_listener_gain', 'i', 'i')
    GET_GAIN = SealAPI.new('get_listener_gain', 'p', 'i')
    SET_POS = SealAPI.new('set_listener_pos', 'iii', 'i')
    GET_POS = SealAPI.new('get_listener_pos', 'ppp', 'i')
    SET_VEL = SealAPI.new('set_listener_vel', 'iii', 'i')
    GET_VEL = SealAPI.new('get_listener_vel', 'ppp', 'i')
    SET_ORIEN = SealAPI.new('set_listener_orien', 'p', 'i')
    GET_ORIEN = SealAPI.new('get_listener_orien', 'p', 'i')


    def gain=(gain)
      CHECK_ERROR[SET_GAIN[[gain].pack('f').unpack('i')[0]]]
      gain
    end

    def gain
      gain_buffer = '    '
      CHECK_ERROR[GET_GAIN[gain_buffer]]
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

  private
    def set_3float(float_tuple, setter)
      CHECK_ERROR[setter[*float_tuple.pack('f*').unpack('i*')]]
      return float_tuple
    end

    def get_3float(getter)
      float_tuple_buffers = Array.new(3) { '    ' }
      CHECK_ERROR[getter[*float_tuple_buffers]]
      return float_tuple_buffers.join.unpack('f*')
    end
  end
end