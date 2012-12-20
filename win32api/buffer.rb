require File.join(File.dirname(__FILE__), 'core')

module Seal
  class Buffer
    include Helper

    INIT_BUF = SealAPI.new('init_buf', 'p', 'i')
    LOAD2BUF = SealAPI.new('load2buf', 'ppi', 'i')
    GET_BUF_SIZE = SealAPI.new('get_buf_size', 'pp', 'i')
    GET_BUF_FREQ = SealAPI.new('get_buf_freq', 'pp', 'i')
    GET_BUF_BPS = SealAPI.new('get_buf_bps', 'pp', 'i')
    GET_BUF_NCHANNELS = SealAPI.new('get_buf_nchannels', 'pp', 'i')

    def initialize(filename, format = Format::UNKNOWN)
      @buffer = '    '
      check_error(INIT_BUF[@buffer])
      input_audio(@buffer, LOAD2BUF, filename, format)
      self
    end

    def load(filename, format = Format::UNKNOWN)
      input_audio(@buffer, LOAD2BUF, filename, format)
      self
    end

    def size
      get_obj_int(@buffer, GET_BUF_SIZE)
    end

    def frequency
      get_obj_int(@buffer, GET_BUF_FREQ)
    end

    def bit_depth
      get_obj_int(@buffer, GET_BUF_BPS)
    end

    def channel_count
      get_obj_int(@buffer, GET_BUF_NCHANNELS)
    end
  end
end