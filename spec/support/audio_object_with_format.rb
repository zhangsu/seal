require 'spec_helper'

shared_examples 'an audio object with format' do

  context 'read from the WAVE file' do
    subject { described_class.new(WAV_PATH) }
    its(:bit_depth) { is_expected.to eq 8 }
    its(:channel_count) { is_expected.to eq 1 }
    its(:frequency) { is_expected.to eq 11_025 }
  end

  context 'read from the Ogg Vorbis file' do
    subject { described_class.new(OV_PATH) }
    its(:bit_depth) { is_expected.to eq 16 }
    its(:channel_count) { is_expected.to eq 1 }
    its(:frequency) { is_expected.to eq 44_100 }
  end

  it 'fails when reading WAVE with OV format specified' do
    expect do
      described_class.new(WAV_PATH, Format::OV)
    end.to raise_error /Ogg Vorbis/
  end

  it 'fails when reading OV with MPG format specified' do
    expect do
      described_class.new(OV_PATH, Format::MPG)
    end.to raise_error /MPEG/
  end
end
