require 'mkmf'
require 'fileutils'
require 'rbconfig'

include FileUtils
include RbConfig

target_os = CONFIG['target_os']
root_dir = '..'
src_dir = "#{root_dir}/src"

$defs << '-DNDEBUG'

def check_library(lib, func)
  raise "#{lib} is missing. See README." unless have_library(lib, func)
end

if target_os =~ /mswin|mingw/
  include_dir = "#{root_dir}/include"
  lib_dir = "#{root_dir}/msvc/lib"
  cp "#{lib_dir}/libmpg123.dll", '.'
  cp "#{lib_dir}/OpenAL32.dll", '.'
  check_library('OpenAL32', 'alcOpenDevice')
  check_library('libmpg123', 'mpg123_init')
else
  include_dir = "#{root_dir}"
  check_library('openal', 'alcOpenDevice')
  check_library('mpg123', 'mpg123_init')
  check_library('pthread', 'pthread_create')
end

# Add source directories.
$VPATH << src_dir << "#{src_dir}/seal"
$VPATH << "#{src_dir}/libogg" << "#{src_dir}/libvorbis"
dir_config('seal', include_dir, lib_dir)
create_makefile('seal', '../src/**/')
