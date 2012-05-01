require_relative 'seal'

include Seal

Seal.startup

EFFECT = ARGV[0]

def print_constants(mod, level = 0)
  mod.constants.each do |const_key|
    const_val = mod.const_get(const_key)
    puts ' ' * level + const_key.to_s
    print_constants(const_val, level + 4) if const_val.is_a? Module
  end
end

puts 'Available reverb presets:'
print_constants(Reverb::Preset)
puts 'Currently rendering: '
puts EFFECT

wolf = Source.new
wolf.buffer = Buffer.new('audio/wolf.ogg')
wolf.position = -5, 5, 0

crow = Source.new
crow.buffer = Buffer.new('audio/crow.ogg')
crow.position = 6, 3, 0

reverb = Reverb.new(Reverb::Preset.module_eval(EFFECT))
slot = EffectSlot.new(reverb)

[wolf, crow].each do |source|
  slot.feed(0, source)
  Thread.new do
    loop do
      source.play if source.state != Source::State::PLAYING
      sleep rand(3) + 5
    end
  end
end

puts "Hit enter to toggle reverb effect"
while $stdin.gets
  if slot.effect
    puts "Reverb off"
    slot.effect = nil
  else
    puts "Reverb on"
    slot.effect = reverb
  end
end

Seal.cleanup
