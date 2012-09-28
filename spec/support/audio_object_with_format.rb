require 'spec_helper'

shared_examples 'an audio object with format' do
  it 'fails reading audio if a wrong format is specified' do
    expect do
      described_class.new(WAV_PATH, Format::OV)
    end.to raise_error /Ogg Vorbis/
    expect do
      described_class.new(OV_PATH, Format::MPG)
    end.to raise_error /MPEG/
  end
end
