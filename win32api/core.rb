require File.join(File.dirname(__FILE__), 'sealapi')

module Seal
  VERSION = SealAPI.new('get_version', 'v', 'p')[]

  get_err_msg = SealAPI.new('get_err_msg', 'i', 'p')
  CHECK_ERROR = lambda do |error_code|
    raise SealError, get_err_msg[error_code] if error_code != 0
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

  module Format
    WAV = :wav
    OV = :ov
    MPG = :mpg
  end

  class SealError < Exception
  end
end