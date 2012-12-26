require File.join(File.dirname(__FILE__), 'core')

module Seal
  class Buffer
    include Helper

    INIT = SealAPI.new('init_buf', 'p', 'i')
    DESTROY = SealAPI.new('destroy_buf', 'p', 'i')
    LOAD = SealAPI.new('load2buf', 'ppi', 'i')
    GET_SIZE = SealAPI.new('get_buf_size', 'pp', 'i')
    GET_FREQ = SealAPI.new('get_buf_freq', 'pp', 'i')
    GET_BPS = SealAPI.new('get_buf_bps', 'pp', 'i')
    GET_NCHANNELS = SealAPI.new('get_buf_nchannels', 'pp', 'i')

    def initialize(filename, format = Format::UNKNOWN)
      @buffer = '    '
      check_error(INIT[@buffer])
      input_audio(@buffer, filename, format, LOAD)
      ObjectSpace.define_finalizer(self, Helper.free(@buffer, DESTROY))
      self
    end

    def load(filename, format = Format::UNKNOWN)
      input_audio(@buffer, filename, format, LOAD)
      self
    end

    def size
      get_obj_int(@buffer, GET_SIZE)
    end

    def frequency
      get_obj_int(@buffer, GET_FREQ)
    end

    def bit_depth
      get_obj_int(@buffer, GET_BPS)
    end

    def channel_count
      get_obj_int(@buffer, GET_NCHANNELS)
    end
  end
end