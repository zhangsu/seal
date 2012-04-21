require_relative 'seal'

Seal.startup

src = Seal::Source.new
src.stream = Seal::Stream.open('audio/pipa.ogg')
src.play

sleep 1 while src.state == Seal::Source::State::PLAYING

Seal.cleanup
