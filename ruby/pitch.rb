require_relative 'seal'

Seal.startup

source = Seal::Source.new
source.stream = Seal::Stream.open('audio/pipa.ogg')
source.play
source.looping = true

FACTOR = 0.01

puts "Enter [ to reduce the pitch by #{FACTOR}."
puts "Enter ] to raise the pitch by #{FACTOR}."
puts "Enter q to quit."

until (c = $stdin.getc) =~ /q/i
  if c =~ /\[/i
    source.pitch -= FACTOR
  elsif c =~ /\]/i
    source.pitch += FACTOR
  end
end

Seal.cleanup
