require './seal'

Audio.startup

src = Audio::Source.new
src.stream = Audio::Stream.open('pipa.ogg')
src.play

sleep 1 while src.state == :playing

Audio.cleanup
