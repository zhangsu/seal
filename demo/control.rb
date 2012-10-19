require 'seal'

include Seal

Seal.startup

pipa = Source.new
people = Source.new
pipa_stream = Stream.open('audio/pipa.ogg')
pipa.stream = pipa_stream
people.stream = Stream.open('audio/people.ogg')
pipa.looping = people.looping = true
people.gain = 0.2

pipa.play
people.play

loop do

  puts 'Choose:'
  puts '1. play'
  puts '2. pause'
  puts '3. stop'
  puts '4. rewind'
  puts '5. detach'
  puts '6. re-attach'
  puts '7. quit'

  c = gets.to_i
  case c
  when 1
    pipa.play
  when 2
    pipa.pause
  when 3
    pipa.stop
  when 4
    pipa.rewind
  when 5
    pipa.stream = nil
  when 6
    pipa.stream = pipa_stream
  when 7
    break
  end

  puts "state: #{ pipa.state }"

end

Seal.cleanup
