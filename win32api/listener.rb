require './core'

module Seal
  class Listener
    SET_GAIN = SealAPI.new('set_listener_gain', 'i', 'i')
    GET_GAIN = SealAPI.new('get_listener_gain', 'p', 'i')
    SET_POSITION = SealAPI.new('set_listener_pos', 'iii', 'i')
    GET_POSITION = SealAPI.new('get_listener_pos', 'ppp', 'i')

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
      CHECK_ERROR[SET_POSITION[*position.pack('f*').unpack('i*')]]
      position
    end

    def position
      position_buffers = Array.new(3) { '    ' }
      CHECK_ERROR[GET_POSITION[*position_buffers]]
      return position_buffers.join.unpack('f*')
    end
  end
end