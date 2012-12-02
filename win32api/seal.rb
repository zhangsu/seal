# Require this script to bind SEAL through Win32API.
#
# This is a fallback solution just in case Ruby API and DL are unavailable.
# Performance-wise, Win32API < DL < Ruby API.
# This is a fallback solution just in case Ruby API and DL are unavailable.
# Performance-wise, Win32API < DL < Ruby API.

require_relative 'sealapi'

module Seal
  VERSION = SealAPI.new('get_version', 'v', 'p')[]

  class << self
    GET_ERR_MSG = SealAPI.new('get_err_msg', 'i', 'p')
    STARTUP = SealAPI.new('startup', 'p', 'i')
    CLEANUP = SealAPI.new('cleanup', 'v', 'v')
    GET_PER_SRC_EFFECT_LIMIT = SealAPI.new('get_per_src_effect_limit',
                                           'v', 'i')

    def startup(device = nil)
      _check_error(STARTUP[device])
    end

    def cleanup
      CLEANUP[]
    end

    def per_source_effect_limit
      GET_PER_SRC_EFFECT_LIMIT[]
    end

    def _check_error(error_code)
      raise SealError, GET_ERR_MSG.call(error_code) if error_code != 0
      nil
    end
  end

  class SealError < Exception
  end

end