require './core'

module Seal
  class Listener
    SET_POSITION = SealAPI.new('set_listener_pos', 'iii', 'i')
    GET_POSITION = SealAPI.new('get_listener_pos', 'ppp', 'i')

    def position=(pos)
      CHECK_ERROR[SET_POSITION[*pos.pack('f*').unpack('i*')]]
      pos
    end

    def position
      position_buffers = Array.new(3) { '    ' }
      CHECK_ERROR[GET_POSITION[*position_buffers]]
      return position_buffers.join.unpack('f*')
    end
  end
end