require 'spec_helper'

describe Buffer do
  describe 'loading' do
    it 'cannot be done if it is being used by sources' do
      error_pattern = /Invalid operation/
      source = Source.new
      another_source = Source.new
      buffer = Buffer.new(WAV_PATH)
      source.buffer = buffer
      another_source.buffer = buffer
      # Two sources are using buffer.
      expect { buffer.load(OV_PATH) }.to raise_error error_pattern
      source.detach
      # One more source is still using buffer.
      expect { buffer.load(OV_PATH) }.to raise_error error_pattern
      another_source.detach
      expect { buffer.load(OV_PATH) }.to_not raise_error
    end
  end
end
