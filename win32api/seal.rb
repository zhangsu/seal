# Require this script to bind SEAL through Win32API.
#
# This is a fallback solution just in case Ruby API and DL are unavailable.
# Performance-wise, Win32API < DL < Ruby API.

current_dir = File.dirname(__FILE__)
require File.join(current_dir, 'core')
require File.join(current_dir, 'listener')
require File.join(current_dir, 'buffer')
require File.join(current_dir, 'effect_slot')
require File.join(current_dir, 'reverb')
require File.join(current_dir, 'source')
require File.join(current_dir, 'stream')