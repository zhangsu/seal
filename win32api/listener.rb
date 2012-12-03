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
      pos = Array.new(3) { ' ' * 4 }
      CHECK_ERROR[GET_POSITION[*pos]]
      return pos.join.unpack('f*')
    end
  end
end