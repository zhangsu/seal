require_relative 'seal'

Seal.startup

src = Seal::Source.new
src.stream = Seal::Stream.open('pipa.ogg')
src.play
src.looping = true

FACTOR = 0.01

puts "Enter [ to reduce the pitch by #{FACTOR}."
puts "Enter ] to raise the pitch by #{FACTOR}."
puts "Enter q to quit."

until (c = $stdin.getc) =~ /q/i
  if c =~ /\[/i
    src.pitch -= FACTOR
  elsif c =~ /\]/i
    src.pitch += FACTOR
  end
end

Seal.cleanup
