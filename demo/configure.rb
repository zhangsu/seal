require 'mkmf'
require 'fileutils'
require 'rbconfig'
include FileUtils
include RbConfig

target_os = CONFIG['target_os']
root_dir = '..'
lib_dir = nil
src_dir = "#{root_dir}/src"

$defs << '-DNDEBUG'

if target_os =~ /mswin|mingw/
  include_dir = "#{root_dir}/include"
  lib_dir = "#{root_dir}/lib"
  cp "#{lib_dir}/libmpg123.dll", '.'
  cp "#{lib_dir}/OpenAL32.dll", '.'
  cp "#{lib_dir}/wrap_oal.dll", '.'
  have_library('OpenAL32', 'alcOpenDevice')
  have_library('libmpg123', 'mpg123_init')
elsif target_os =~ /linux/
  include_dir = "#{root_dir}"
  have_library('openal', 'alcOpenDevice')
  have_library('mpg123', 'mpg123_init')
else
  puts 'This configuration does not support the current operating system.'
  exit
end

# Add source directories.
$VPATH << src_dir << "#{src_dir}/seal"
$VPATH << "#{src_dir}/libogg" << "#{src_dir}/libvorbis"
dir_config('seal', include_dir, lib_dir)
create_makefile('seal', '../src/**')
