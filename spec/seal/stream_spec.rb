require 'spec_helper'

include Source::State

describe Stream do
  let(:source) do
    Source.new.tap { |source| source.stream = Stream.new(WAV_PATH) }
  end

  it_behaves_like 'an audio object with format'

  specify 'open is equivalent to new' do
    klass = described_class
    class << klass; alias unit_test_open new; end
    expect(klass.method(:open)).to eq klass.method(:unit_test_open)
    class << klass; remove_method :unit_test_open; end
  end

  context 'that are closed' do
    subject do
      Stream.new(WAV_PATH).tap { |stream| stream.close }
    end

    its(:bit_depth) { is_expected.to eq 16 }
    its(:channel_count) { is_expected.to eq 1 }
    its(:frequency) { is_expected.to eq 0 }
  end

  example 'rewinding prevents source from stopping' do
    source.play
    6.times do
      sleep(0.1)
      source.stream.rewind
    end
    expect(source.state).to be PLAYING
  end

  it 'can be closed even when used by sources' do
    expect do
      source.play
      source.stream.close
      sleep 0.5
    end.to_not raise_error
  end
end
