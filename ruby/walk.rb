require 'scanf'
require_relative 'seal'

Seal.startup

RATE = 1.0
UNIT_PER_100MS = RATE / 10.0

puts "
Note: spatialization only works on monophonic (single-channel) sound.

You, the listener, are at (0, 0, 0) in an Euclidean space.
The sound source is currently also at (0, 0, 0).
You move around by entering a new destination coordinate, which is represented
by a 3-tuple of real numbers separated by white spaces.

You will be moving at a rate of #{RATE} unit/sec.
"

src = Seal::Source.new
src.stream = Seal::Stream.new('audio/people.ogg')
src.looping = true
src.play
listener = Seal.listener

loop do
  print 'Enter new destination: '
  dest = scanf('%f%f%f')
  $walker.kill if $walker
  origin = listener.position
  printf("Walking from (%.2f, %.2f, %.2f) to (%.2f, %.2f, %.2f).\n",
         *origin, *dest)
  $walker = Thread.new do
    disp = []
    3.times { |i| disp[i] = dest[i] - origin[i] }
    return if disp.all? &:zero?
    scalar = UNIT_PER_100MS / Math.sqrt(disp[0]**2 + disp[1]**2 + disp[2]**2)
    velocity = disp.map { |component| scalar * component }
    listener.velocity = velocity
    begin
      3.times { |i| origin[i] += velocity[i] }
      listener.position = origin
      sleep(0.1)
    end until (dest[0] - origin[0]).abs < velocity[0].abs
    listener.position = dest
  end
end