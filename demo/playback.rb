require 'seal'

Seal.startup

source = Seal::Source.new
source.stream = Seal::Stream.open('audio/pipa.ogg')
source.play

sleep 1 while source.state == Seal::Source::State::PLAYING

Seal.cleanup
