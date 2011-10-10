require './seal'

Audio.startup

src = Audio::Source.new
src.stream = Audio::Stream.open('pipa.ogg')
src.play
src.looping = true

Thread.new do
  loop do
      src.update
      sleep(0.1)
  end
end

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

Audio.cleanup