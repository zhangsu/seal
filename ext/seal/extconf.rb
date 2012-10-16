require 'mkmf'
require 'fileutils'
require 'rbconfig'

include FileUtils
include RbConfig

target_os = CONFIG['target_os']
root_dir = File.join(File.dirname(__FILE__), '..', '..')
src_dir = File.join root_dir, 'src'
mpg123_dir = File.join root_dir, 'mpg123'
mpg123_src_dir = File.join mpg123_dir, 'src'
mpg123_lib_dir = File.join mpg123_src_dir, 'libmpg123', '.libs'

$defs << '-DNDEBUG'
$LDFLAGS << ' -s'

unless File.exists?(File.join(mpg123_src_dir, 'config.h'))
  puts 'Building libmpg123...'
  cd(mpg123_dir) { `sh ./configure --with-pic --disable-shared` }
  cd(mpg123_src_dir) { `make` }
end
find_library 'mpg123', 'mpg123_init', mpg123_lib_dir

def check_library(lib, func)
  raise "#{lib} is missing. See README." unless have_library(lib, func)
end

if target_os =~ /mswin|mingw/
  include_dir = "#{root_dir}/include"
  lib_dir = File.join root_dir, 'msvc', 'lib'
  cp File.join(lib_dir, 'OpenAL32.dll'), '.'
  # Prepare binary for demo.
  cp File.join(lib_dir, 'OpenAL32.dll'), File.join(root_dir, 'demo')
  # Prepare binary for tests.
  cp File.join(lib_dir, 'OpenAL32.dll'), root_dir
  check_library('OpenAL32', 'alcOpenDevice')
else
  include_dir = "#{root_dir}"
  check_library('openal', 'alcOpenDevice')
  check_library('pthread', 'pthread_create')
end

# Add source directories.
$VPATH << src_dir << File.join(src_dir, 'seal') <<
          File.join(src_dir, 'libogg') <<
          File.join(src_dir, 'libvorbis')
dir_config('seal', include_dir, lib_dir)

create_makefile('seal', File.join(src_dir, '**', ''))
