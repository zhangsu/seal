Gem::Specification.new do |s|
  s.name      = 'seal'
  s.version   = '0.1.0'
  s.summary   = 'An OpenAL-based 3D audio library'
  s.authors   = ['Su Zhang']
  s.email     = 'me@zhang.su'
  s.homepage  = 'https://github.com/zhangsu/seal'
  s.license   = 'WTFPL'

  s.files = `git ls-files`.split.select do |filename|
    filename =~ /^(ext|include|mpg123|spec|src)[\/\\]/
  end + ['LICENSE', 'README.md', 'msvc/lib/OpenAL32.lib']
  s.test_files  = Dir.glob('spec/**/*.rb')
  s.extensions  = ['ext/seal/extconf.rb']

  s.rdoc_options  = ['src/rubyext.c']
  s.extra_rdoc_files = ['src/rubyext.c']

  s.required_ruby_version = '~> 1.9'
  s.requirements = ['libopenal', 'a sound card']
  s.add_development_dependency 'rake-compiler'
  s.add_development_dependency 'rspec', '~> 2'
  s.add_development_dependency 'guard-rspec'
  s.add_development_dependency 'yard'
  s.add_development_dependency 'rb-inotify'

  s.post_install_message  = 'Enjoy!'
  s.description = <<-DESCRIPTION
    Seal is a library for 3D audio rendering and manipulation, supporting
    effects such as direction and distance attenuation, the simulation of the
    Doppler effect and reverberation. It is built on top of OpenAL, adding
    support for audio streaming and audio formats like Ogg Vorbis, MPEG Audio
    and WAVE.
  DESCRIPTION
end
