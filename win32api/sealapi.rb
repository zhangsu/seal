require 'win32api' unless defined? Win32API

LIB_DIR = ARGV[0] || '.'

class SealAPI < Win32API
  def initialize(func, *args)
    super("#{LIB_DIR}\\seal", "seal_#{func}", *args)
  end

  def [](*args)
    call(*args)
  end unless method_defined? :[]
end
