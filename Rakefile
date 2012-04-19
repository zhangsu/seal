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
  $, = ' '
  # Project directory relative to Makefile.
  # Assuming all Makefiles are at the same (directory tree) level.
  PROJ_DIR          = File.join('..', '..', '..')
  INCLUDE_DIR       = File.join(PROJ_DIR, 'include')
  LIB_DIR           = File.join(PROJ_DIR, 'lib')
  MAKEFILE_TEMPLATE = File.join(PROJ_DIR, 'Makefile.erb')
  # Library input list for Win32.
  LIBS              = [File.join(LIB_DIR, 'OpenAL32.lib'),
                       File.join(LIB_DIR, 'libmpg123.lib')].join
  DLLS              = ['OpenAL32.dll', 'libmpg123.dll']

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

      def dir_timestamp
        File.join(self, 'timestamp')
      end

      def second_token
        self =~ /:\s+\S+[\s\\]+(\S+)/
        $1
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

    filenames   = Dir.chdir('src') { Dir['*/*.c'] }
    OBJECTS     = filenames.map(&:object_filename).join

    RULE_GROUPS = {}
    workers = []
    # Pick one of the directories to generate all the rules.
    Dir.chdir File.join('make', 'gcc', 'unix-like')
    filenames.each do |filename|
      workers << Thread.new do
        rule = filename.rule
        key = rule.target_dirname
        rule.sub!(/:/, ': ' + key.dir_timestamp)
        Thread.exclusive do
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

  # Assuming UN*X shell.
  namespace :gcc do |; gcc_options|
    gcc_options = {
      cc:     'gcc',
      linker: 'gcc',
      flags:  '-Wextra -O3',
      cflags: "-I#{INCLUDE_DIR} -DNDEBUG -c",
      lflags: '-shared',
      coflag: '-o ',
      loflag: '-o ',
      mkdir:  'mkdir -p',
      rm:     'rm -rf',
      rmdir:  'rmdir',
    }

    desc 'Generate Makefile for GCC on all platforms'
    task :all => [:unix_like, :win32]

    desc 'Generate Makefile for GCC on unix-like operating systems'
    task :unix_like => :prepare do |t; options|
      report(t)
      options = gcc_options.merge(
        libs:   '-lopenal -lmpg123',
        dlls:   [],
        output: 'libseal.so',
      )
      options[:cflags] += ' -fPIC'
      make_makefile_in(File.join('make', 'gcc', 'unix-like'), options)
    end

    desc 'Generate Makefile for GCC on Win32'
    task :win32 => :prepare do |t; options|
      report(t)
      options = gcc_options.merge(
        libs:     LIBS,
        dlls:     DLLS.map { |dll| File.join(LIB_DIR, dll) },
        output:   'seal.dll',
        cp:       'cp',
      )
      options[:lflags] += ' $(PROJDIR)/msvc/seal/exports.def'
      make_makefile_in(File.join('make', 'gcc', 'win32'), options)
    end
  end

  # Assuming MS-DOS command prompt.
  namespace :msvc do |; msvc_options|
    msvc_options = {
      cc:         'cl',
      linker:     'link',
      cflags:     '/c /Ob2ity /Gd /GL /Gy /MD /DNDEBUG '\
                  "/I #{INCLUDE_DIR} /TC /nologo",
      lflags:     '/DLL /LTCG /OPT:REF /OPT:ICF=3 /MANIFEST /NOLOGO '\
                  '/DEF:$(PROJDIR)/msvc/seal/exports.def',
      libs:       LIBS,
      dlls:       DLLS.map { |dll| LIB_DIR.gsub(/\/|$/, '\\') + dll },
      coflag:     '/Fo',
      loflag:     '/OUT:',
      output:     'seal.dll',
      after_link: 'mt -nologo -manifest $@.manifest -outputresource:$@;1',
      mkdir:      'mkdir',
      rm:         'del /F /S /Q',
      rmdir:      'rmdir',
      cp:         'copy /Y',
    }

    desc 'Generate Makefile for MSVC on all platforms'
    task :all => :win32

    desc 'Generate Makefile for MSVC on all Win32'
    task :win32 => :prepare do |t; options|
      report(t)
      make_makefile_in(File.join('make', 'msvc', 'win32'), msvc_options)
    end
  end
end

