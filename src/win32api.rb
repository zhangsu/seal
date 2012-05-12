# Require this script to bind SEAL through Win32API.
# This is a fallback solution just in case Ruby API and DL are unavailable.
# Performance-wise, Win32API < DL < Ruby API.

require 'win32api'

class SealError < Exception
end

module Seal
  class << self
    GET_ERR_MSG = Win32API.new('seal', 'seal_get_err_msg', 'i', 'p')
    STARTUP = Win32API.new('seal', 'seal_startup', 'p', 'i')
    CLEANUP = Win32API.new('seal', 'seal_cleanup', 'v', 'v')

    def check_error(error_code)
      raise SealError, GET_ERR_MSG.call(error_code) if error_code != 0
      nil
    end

    def startup(device = nil)
      check_error(STARTUP.call(device))
    end

    def cleanup
      CLEANUP.call
    end
  end
end