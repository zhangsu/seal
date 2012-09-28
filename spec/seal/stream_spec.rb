require 'spec_helper'
require 'audio_object_with_format'

describe Stream do
  it_behaves_like 'an audio object with format'

  context 'that are closed' do
    subject do
      Stream.new(WAV_PATH).tap { |stream| stream.close }
    end

    its(:bit_depth) { should eq 16 }
    its(:channel_count) { should eq 1 }
    its(:frequency) { should eq 0 }
  end
end
