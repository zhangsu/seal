require 'seal'

include Seal

Seal.startup

source = Source.new
source.buffer = Buffer.new('audio/siren.wav')
source.looping = true
source.play

def move(source, steps = 100)
  steps.times do
    source.position = source.position.zip(source.velocity).map { |a, b| a + b }
    p source.position
    sleep(0.1)
  end
end

loop do
  source.velocity = 1, 1, 0
  move(source)
  source.velocity = -1, -1, 0
  move(source)
end