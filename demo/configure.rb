require 'mkmf'
require 'fileutils'
require 'rbconfig'
include FileUtils
include Config

target_os = CONFIG['target_os']
root_dir = '..'
include_dir = "#{root_dir}/include"
lib_dir = nil
src_dir = "#{root_dir}/src"

$defs << '-DNDEBUG'

if target_os =~ /mswin|mingw/
  lib_dir = "#{root_dir}/lib/win32"
  cp "#{lib_dir}/libmpg123.dll", '.'
  cp "#{lib_dir}/OpenAL32.dll", '.'
  cp "#{lib_dir}/wrap_oal.dll", '.'
  have_library('OpenAL32', 'alcOpenDevice')
elsif target_os =~ /linux/
  lib_dir = "#{root_dir}/lib/linux32"
  # TODO
else
  puts 'This configuration does not support the current operating system.'
  exit
end

# Add source directories.
$VPATH << src_dir << "#{src_dir}/seal"
$VPATH << "#{src_dir}/libogg" << "#{src_dir}/libvorbis"
dir_config('seal', include_dir, lib_dir)
have_library('libmpg123', 'mpg123_init')
create_makefile('seal', '../src/**')