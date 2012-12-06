require 'win32api' unless defined? Win32API

LIB_DIR = '.' unless defined? LIB_DIR

class SealAPI < Win32API
  def initialize(func, *args)
    super(File.join(LIB_DIR, 'seal'), "seal_#{func}", *args)
  end

  def [](*args)
    call(*args)
  end unless method_defined? :[]
end
