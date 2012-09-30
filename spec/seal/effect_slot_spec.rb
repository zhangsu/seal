require 'spec_helper'

describe EffectSlot do
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
end
