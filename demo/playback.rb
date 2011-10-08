require './seal'

Audio.startup

src = Audio::Source.new
src.stream = Audio::Stream.open('pipa.ogg')
src.play
while src.state == :playing
  src.update
  sleep(0.01)
end

Audio.cleanup