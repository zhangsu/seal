require File.join(File.dirname(__FILE__), 'core')

module Seal
  class Stream
    include Helper

    OPEN = SealAPI.new('open_stream', 'ppi', 'i')
    CLOSE = SealAPI.new('close_stream', 'p', 'i')
    REWIND = SealAPI.new('rewind_stream', 'p', 'i')

    class << self
      alias open new
    end

    def initialize(filename, format = Format::UNKNOWN)
      @stream = '    ' * 5
      input_audio(@stream, OPEN, filename, format)
      ObjectSpace.define_finalizer(self, Helper.free(@stream, CLOSE))
      self
    end

    def frequency
      field(4)
    end

    def bit_depth
      field(2)
    end

    def channel_count
      field(3)
    end

    def rewind
      check_error(REWIND[@stream])
    end

    def close
      check_error(CLOSE[@stream])
    end

  private
    def field(index)
      @stream[index * 4, 4].unpack('i')[0]
    end
  end
end