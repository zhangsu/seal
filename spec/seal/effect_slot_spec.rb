require 'spec_helper'

describe EffectSlot do
  before(:each) { GC.start }

  describe 'by default' do
    its(:auto?) { should be_true }
    its(:effect) { should be_nil }
    its(:gain) { should eq 1.0 }
  end

  it 'can initialize with an effect' do
    reverb = Reverb.new
    effect_slot = EffectSlot.new(reverb)
    effect_slot.effect.should be reverb
  end

  context 'with valid attributes' do
    it 'has a gain in [0, 1.0]' do
      error_pattern = /Invalid parameter value/
      effect_slot = subject
      effect_slot.gain = 0.7667
      effect_slot.gain.should be_within(TOLERANCE).of 0.7667
      effect_slot.gain = 1
      effect_slot.gain.should eq 1
      effect_slot.gain = 0
      expect { effect_slot.gain = -3.2 }.to raise_error error_pattern
      expect { effect_slot.gain = 1.01 }.to raise_error error_pattern
      expect { effect_slot.gain = 3.9 }.to raise_error error_pattern
      effect_slot.gain.should eq 0
    end
  end

  it 'has a limited number of sources it can feed concurrently' do
    effect_slot = subject
    sources = []
    Seal.per_source_effect_limit.times do |i|
      sources << Source.new
      effect_slot.feed(i, sources[-1])
    end
    expect { effect_slot.feed(sources.size, Source.new) }.to raise_error \
      /Invalid parameter value/
  end
end
