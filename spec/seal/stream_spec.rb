require 'spec_helper'

include Source::State

describe Stream do
  let(:source) do
    Source.new.tap { |source| source.stream = Stream.new(WAV_PATH) }
  end

  it_behaves_like 'an audio object with format'

  context 'that are closed' do
    subject do
      Stream.new(WAV_PATH).tap { |stream| stream.close }
    end

    its(:bit_depth) { should eq 16 }
    its(:channel_count) { should eq 1 }
    its(:frequency) { should eq 0 }
  end

  example 'rewinding prevents source from stopping' do
    source.play
    3.times do
      sleep(0.2)
      source.stream.rewind
    end
    source.state.should be PLAYING
  end

  it 'can be closed even when used by sources' do
    expect do
      source.play
      source.stream.close
      sleep 0.5
    end.to_not raise_error
  end
end
