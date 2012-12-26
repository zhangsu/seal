require 'win32api' unless defined? Win32API

LIB_DIR = '.' unless defined? LIB_DIR

class SealAPI < Win32API
  STRCPY_S = Win32API.new('msvcrt', 'strcpy_s', 'pll', 'i')

  def initialize(func, arg_types, return_type = 'i', *args)
    @return_string = return_type == 'p'
    library = File.join(LIB_DIR, 'seal')
    super(library, "seal_#{func}", arg_types, return_type, *args)
  end

  def [](*args)
    result = call(*args)
    if @return_string and result.is_a? Integer
      # String pointer is returned to Ruby as an integer even though we
      # specified 'p' as the return value - possibly a bug in Ruby 1.9.3's
      # Win32API implementation. Work around it.
      message_buffer = ' ' * 128
      STRCPY_S.call(message_buffer, message_buffer.size, result)
      return message_buffer.strip
    end
    result
  end unless method_defined? :[]
end
