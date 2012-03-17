require_relative 'seal'

Seal.startup

src = Seal::Source.new
src.stream = Seal::Stream.open('pipa.ogg')
src.play

sleep 1 while src.state == :playing

Seal.cleanup
