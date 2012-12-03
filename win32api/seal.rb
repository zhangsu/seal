# Require this script to bind SEAL through Win32API.
#
# This is a fallback solution just in case Ruby API and DL are unavailable.
# Performance-wise, Win32API < DL < Ruby API.

require './core'
require './listener'