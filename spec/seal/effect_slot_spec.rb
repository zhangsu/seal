require 'spec_helper'

describe EffectSlot do
  before(:each) { GC.start }

  describe 'by default' do
    its(:auto?) { should be_true }
    its(:effect) { should be_nil }
    its(:gain) { should eq 1.0 }
  end

  it_validates 'the boolean attribute', :auto
  it_validates 'the float attribute', :gain, "[0, 1]"
  it_defines 'boolean reader aliases', [:auto]

  it 'can initialize with an effect' do
    reverb = Reverb.new
    effect_slot = EffectSlot.new(reverb)
    effect_slot.effect.should be reverb
  end

  it 'can set an effect' do
    effect_slot = subject
    reverb = Reverb.new
    effect_slot.effect = reverb
    effect_slot.effect.should be reverb
  end
end
