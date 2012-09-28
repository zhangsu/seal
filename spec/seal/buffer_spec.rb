require 'spec_helper'
require 'audio_object_with_format'

describe Buffer do
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

  it_behaves_like 'an audio object with format'
end
