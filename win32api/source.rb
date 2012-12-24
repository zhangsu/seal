current_dir = File.dirname(__FILE__)
[
  'core',
  'buffer',
  'stream'
].each { |mod| require File.join(current_dir, mod) }

module Seal
  class Source
    INIT = SealAPI('init_src', 'p', 'i')
    DESTROY = SealAPI('destroy_src', 'p', 'i')

    def initialize
      @source = '    ' * 5
      INIT[@source]
      ObjectSpace.define_finalizer(self, Helper.free(@source, DESTROY))
    end

    module State
      INITIAL = 0
      PLAYING = 1
      PAUSED = 2
      STOPPED = 3
    end

    module Type
      UNDETERMINED = 0
      STATIC = 1
      STREAMING = 2
    end
  end
end