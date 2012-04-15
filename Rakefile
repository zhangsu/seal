require 'erb'

desc 'Run the default task'
task :default => :all

desc 'Run all tasks'
task :all => :'mf:all'

def report(task)
  puts "Running #{task.name}..."
end

# Makefile generation tasks.
# Requires GCC for dependency generation.
namespace :mf do
  # Project directory relative to Makefile.
  # Assuming all Makefiles are at the same (directory tree) level.
  PROJ_DIR          = File.join('..', '..', '..')
  INCLUDE_DIR       = File.join(PROJ_DIR, 'include')
  LIB_DIR           = File.join(PROJ_DIR, 'lib')
  MAKEFILE_TEMPLATE = File.join(PROJ_DIR, 'Makefile.erb')

  task :prepare do |t|
    report(t)

    class String
      def target_dirname
        File.dirname(self[/^.+?(?=:)/])
      end

      def object_filename
        sub(/\.c$/, '.o')
      end

      def rule
        `gcc -I#{INCLUDE_DIR} -MM -MT #{object_filename} \
          #{File.join(PROJ_DIR, 'src', self)}`.strip
      end
    end

    def make_makefile(options)
      open 'Makefile', 'wb' do |f|
        template = File.read(MAKEFILE_TEMPLATE)
        f << ERB.new(template).result(binding)
      end
    end

    def make_makefile_in(dirname, options)
      Dir.chdir(dirname) { make_makefile(options) }      
    end

    $, = ' '
    filenames   = Dir.chdir('src') { Dir['*/*.c'] }
    OBJECTS     = filenames.map(&:object_filename).join
    # Library input list for Win32.
    LIBS        = [File.join(LIB_DIR, 'OpenAL32.lib'),
                   File.join(LIB_DIR, 'libmpg123.lib')].join

    RULE_GROUPS = {}
    workers = []
    # Pick one of the directories to generate all the rules.
    Dir.chdir File.join('make', 'gcc', 'linux')
    filenames.each do |filename|
      workers << Thread.new do
        rule = filename.rule
        Thread.exclusive do
          key = rule.target_dirname
          RULE_GROUPS[key] ||= []
          RULE_GROUPS[key] << rule
        end
      end
    end
    workers.each(&:join)
    Dir.chdir File.join('..', '..', '..')
  end

  desc 'Generate Makefile for all compilers on all platforms'
  task :all => [:'gcc:all', :'msvc:all']

  namespace :gcc do |; options|
    gcc_options = {
      cc:     'gcc',
      linker: 'gcc',
      flags:  '-Wextra -O3',
      cflags: "-I#{INCLUDE_DIR} -DNDEBUG -c",
      lflags: '-shared',
      mkdir:  'mkdir',
      rm:     'rm -rf',
      cp:     'cp',
      oflag:  '-o ',
    }

    desc 'Generate Makefile for GCC on all platforms'
    task :all => [:linux, :win32]

    desc 'Generate Makefile for GCC on Linux'
    task :linux => :prepare do |t|
      report(t)
      
      options = gcc_options.merge(
        libs:   '-lopenal -lmpg123',
        output: 'libseal.so',
      )
      options[:cflags] += ' -fPIC'
      make_makefile_in(File.join('make', 'gcc', 'linux'), options)
    end

    desc 'Generate Makefile for GCC on Win32'
    task :win32 => :prepare do |t|
      report(t)
      options = gcc_options.merge(
        libs:   LIBS,
        output: 'seal.dll',
      )
      make_makefile_in(File.join('make', 'gcc', 'win32'), options)
    end
  end

  namespace :msvc do
    msvc_options = {
      cc:     'cl',
      linker: 'link',
      flags:  '',
      cflags: "",
      lflags: '',
      mkdir:  'mkdir',
      rm:     'del',
      cp:     'copy',
      oflag:  '/Fo',
    }

    desc 'Generate Makefile for MSVC on all platforms'
    task :all => :win32

    desc 'Generate Makefile for MSVC on all Win32'
    task :win32 => :prepare do |t|
      report(t)
      options = msvc_options.merge(
        libs:   LIBS,
        output: 'seal.dll',
      )
      make_makefile_in(File.join('make', 'msvc', 'win32'), options)
    end
  end
end

