require File.join(File.dirname(__FILE__), 'sealapi')

module Seal
  # Define helpers used by Win32API binding code.
  get_err_msg = SealAPI.new('get_err_msg', 'i', 'p')
  CHECK_ERROR = lambda do |error_code|
    raise SealError, get_err_msg[error_code] if error_code != 0
    nil
  end

  INPUT_AUDIO = lambda do |media, input, filename, format|
    CHECK_ERROR[input[media, filename, format]]
  end

  get_obj_attr = lambda do |obj, get, type|
    buffer = '    '
    CHECK_ERROR[get[obj, buffer]]
    buffer.unpack(type)[0]
  end

  GET_OBJ_INT = lambda do |obj, get|
    get_obj_attr[obj, get, 'i']
  end


  VERSION = SealAPI.new('get_version', 'v', 'p')[]

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

  module Format
    UNKNOWN = 0
    WAV = 1
    OV = 2
    MPG = 3
  end

  class SealError < Exception
  end
end