require File.join(File.dirname(__FILE__), 'sealapi')

module Seal
  module Helper
    GET_ERR_MSG = SealAPI.new('get_err_msg', 'i', 'p')

  private
    def check_error(error_code)
      raise SealError, GET_ERR_MSG[error_code] if error_code != 0
      nil
    end

    def input_audio(media, inputter, filename, format)
      check_error(inputter[media, filename, format])
    end

    def get_obj_attr(obj, getter, type)
      buffer = '    '
      check_error(getter[obj, buffer])
      buffer.unpack(type)[0]
    end

    def get_obj_int(obj, getter)
      get_obj_attr(obj, getter, 'i')
    end
  end

  VERSION = SealAPI.new('get_version', 'v', 'p')[]

  class << self
    include Helper

    STARTUP = SealAPI.new('startup', 'p', 'i')
    CLEANUP = SealAPI.new('cleanup', 'v', 'v')
    GET_PER_SRC_EFFECT_LIMIT =
      SealAPI.new('get_per_src_effect_limit', 'v', 'i')

    def startup(device = nil)
      check_error(STARTUP[device])
    end

    def cleanup
      CLEANUP[]
    end

    def per_source_effect_limit
      GET_PER_SRC_EFFECT_LIMIT[]
    end
  end

  module Format
    UNKNOWN = 0
    WAV = 1
    OV = 2
    MPG = 3
  end

  class SealError < Exception
  end
end