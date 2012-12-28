require File.join(File.dirname(__FILE__), 'seal_api')

module Seal
  module Helper
    GET_ERR_MSG = SealAPI.new('get_err_msg', 'i', 'p')

    class << self
      def define_enum(mod, constants, start_value = 0)
        constants.each_with_index do |constant, index|
          mod.const_set(constant, start_value + index)
        end
      end

      # Returns a destructor for a native Seal object. This is most likely
      # called in the initializer method, but we cannot define the proc handler
      # there because that will capture the binding of the implicit `self` (due
      # to the nature of closure), which makes the object that `self` refers to
      # unrecyclable.
      def free(obj, destroyer)
        lambda { |object_id| destroyer[obj] }
      end
    end

  private
    def check_error(error_code)
      raise SealError, GET_ERR_MSG[error_code] if error_code != 0
      nil
    end

    def input_audio(media, filename, format, inputter)
      check_error(inputter[media, filename, format])
    end

    def set_obj_int(obj, int, setter)
      check_error(setter[obj, int])
      int
    end

    def get_obj_int(obj, getter)
      buffer = '    '
      check_error(getter[obj, buffer])
      buffer.unpack('i')[0]
    end

    def set_obj_char(obj, bool, setter)
      set_obj_int(obj, bool ? 1 : 0, setter)
    end

    def get_obj_char(obj, getter)
      buffer = ' '
      check_error(getter[obj, buffer])
      buffer.unpack('c')[0] != 0
    end

    # Win32API does not support float argument type, need to pass as integer.
    def set_obj_float(obj, float, setter)
      check_error(setter[obj, [float].pack('f').unpack('i')[0]])
      float
    end

    def get_obj_float(obj, getter)
      float_buffer = '    '
      check_error(getter[obj, float_buffer])
      float_buffer.unpack('f')[0]
    end
  end

  VERSION = SealAPI.new('get_version', 'v', 'p')[]

  class << self
    include Helper

    STARTUP = SealAPI.new('startup', 'p')
    CLEANUP = SealAPI.new('cleanup', 'v', 'v')
    GET_PER_SRC_EFFECT_LIMIT = SealAPI.new('get_per_src_effect_limit', 'v')

    def startup(device = nil)
      check_error(STARTUP[device ? device : 0])
    end

    def cleanup
      CLEANUP[]
    end

    def per_source_effect_limit
      GET_PER_SRC_EFFECT_LIMIT[]
    end
  end

  module Format
    Helper.define_enum(self, [
      :UNKNOWN,
      :WAV,
      :OV,
      :MPG
    ])
  end

  class SealError < Exception
  end
end