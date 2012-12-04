require './sealapi'

module Seal
  VERSION = SealAPI.new('get_version', 'v', 'p')[]

  get_err_msg = SealAPI.new('get_err_msg', 'i', 'p')
  strcpy_s = Win32API.new('msvcrt', 'strcpy_s', 'pll', 'i')
  CHECK_ERROR = lambda do |error_code|
    if error_code != 0
      message = get_err_msg[error_code]
      if message.is_a? Integer
        # String pointer is returned to Ruby as an integer even though we
        # specified 'p' as the return value - possibly a bug in Ruby 1.9.3's
        # Win32API implementation. Work around it.
        message_buffer = ' ' * 128
        strcpy_s.call(message_buffer, message_buffer.size, message)
        message = message_buffer.strip
      end
      raise SealError, message
    end
    nil
  end

  class << self
    STARTUP = SealAPI.new('startup', 'p', 'i')
    CLEANUP = SealAPI.new('cleanup', 'v', 'v')
    GET_PER_SRC_EFFECT_LIMIT =
      SealAPI.new('get_per_src_effect_limit', 'v', 'i')

    def startup(device = nil)
      CHECK_ERROR[STARTUP[device]]
    end

    def cleanup
      CLEANUP[]
    end

    def per_source_effect_limit
      GET_PER_SRC_EFFECT_LIMIT[]
    end
  end

  class SealError < Exception
  end
end