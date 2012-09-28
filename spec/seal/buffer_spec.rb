require 'spec_helper'

describe Buffer do
  it_behaves_like 'an audio object with format'

  describe 'size after reading' do
    context 'from the WAVE file' do
      subject { Buffer.new(WAV_PATH) }
      its(:size) { should eq 2250 }
    end

    context 'from the Ogg Vorbise file' do
      subject { Buffer.new(OV_PATH) }
      its(:size) { should eq 85342 }
    end
  end

  it 'cannot be changed if it is being used by a source' do
    error_pattern = /Invalid operation/
    source = Source.new
    another_source = Source.new
    buffer = Buffer.new(WAV_PATH)

    source.buffer = buffer
    another_source.buffer = buffer
    # Being used by two sources.
    expect { buffer.load(WAV_PATH) }.to raise_error error_pattern

    source.buffer = nil
    # Still being used by one more source.
    expect { buffer.load(WAV_PATH) }.to raise_error error_pattern

    another_source.buffer = nil
    # Not being used by any source.
    expect { buffer.load(WAV_PATH) }.to_not raise_error
  end
end
