# Require this script to bind SEAL through Win32API.
#
# This is a fallback solution just in case Ruby API and DL are unavailable.
# Performance-wise, Win32API < DL < Ruby API.

current_dir = File.dirname(__FILE__)
[
  'core',
  'listener',
  'buffer',
  'effect_slot',
  'reverb',
  'source',
  'stream'
].each { |mod| require File.join(current_dir, mod) }