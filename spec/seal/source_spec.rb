require 'spec_helper'

include Source::State
include Source::Type

describe Source do
  let(:source) { Source.new }
  let(:stream) { Stream.new(WAV_PATH) }
  let(:buffer) { Buffer.new(WAV_PATH) }

  it_behaves_like 'a movable object'

  describe 'by default' do
    its(:auto) { is_expected.to be_truthy }
    its(:buffer) { is_expected.to be_nil }
    its(:chunk_size) { is_expected.to eq 36_864 }
    its(:gain) { is_expected.to be_within(TOLERANCE).of(1.0) }
    its(:looping) { is_expected.to be_falsey }
    its(:pitch) { is_expected.to be_within(TOLERANCE).of(1.0) }
    its(:queue_size) { is_expected.to eq 3 }
    its(:relative) { is_expected.to be_falsey }
    its(:state) { is_expected.to be INITIAL }
    its(:stream) { is_expected.to be_nil }
    its(:type) { is_expected.to be UNDETERMINED }
  end

  it_validates 'the boolean attribute', :relative
  it_validates 'the boolean attribute', :auto
  it_validates 'the bounded float attribute', :pitch, "[0, +inf.)"
  it_validates 'the bounded float attribute', :gain, "[0, +inf.)"
  it_defines 'boolean reader aliases', %i(auto relative looping)

  it 'validates its queue size is in [2, 63]' do
    error_pattern = /Invalid parameter value/
    expect { source.queue_size = -130 }.to raise_error error_pattern
    expect { source.queue_size = 0 }.to raise_error error_pattern
    expect { source.queue_size = 1 }.to raise_error error_pattern
    source.queue_size = 2
    expect(source.queue_size).to eq 2
    source.queue_size = 32
    expect(source.queue_size).to eq 32
    source.queue_size = 63
    expect(source.queue_size).to eq 63
    expect { source.queue_size = 64 }.to raise_error error_pattern
    expect { source.queue_size = 1203 }.to raise_error error_pattern
    expect(source.queue_size).to eq 63
  end

  it 'validates it has an auto-adjusting chunk size in [9216, 16773120]' do
    error_pattern = /Invalid parameter value/
    expect { source.chunk_size = 0 }.to raise_error error_pattern
    expect { source.chunk_size = 432 }.to raise_error error_pattern
    expect { source.chunk_size = 9215 }.to raise_error error_pattern
    source.chunk_size = 9216
    expect(source.chunk_size).to eq 9216
    # It should be automatically adjusted to a smaller multiple of 9216.
    source.chunk_size = 9217
    expect(source.chunk_size).to eq 9216
    source.chunk_size = 32_768
    expect(source.chunk_size).to eq 27_648
    source.chunk_size = 294_912
    expect(source.chunk_size).to eq 294_912
    source.chunk_size = 16_773_119
    expect(source.chunk_size).to eq 16_763_904
    source.chunk_size = 16_773_120
    expect(source.chunk_size).to eq 16_773_120
    expect { source.chunk_size = 16_773_121 }.to raise_error error_pattern
    expect { source.chunk_size = 234_923_428 }.to raise_error error_pattern
  end

  context 'with a buffer' do
    before(:each) { source.buffer = buffer }

    it 'cannot also have a stream unless buffer is detached' do
      error_pattern = /attach a stream to a static source/
      expect { source.stream = stream }.to raise_error error_pattern
      source.play
      expect { source.stream = stream }.to raise_error error_pattern
      source.pause
      expect { source.stream = stream }.to raise_error error_pattern
      source.stop
      expect { source.stream = stream }.to raise_error error_pattern
      source.buffer = nil
      expect { source.stream = stream }.to_not raise_error
    end

    it 'cannot change its buffer while playing' do
      source.play
      expect { source.buffer = buffer }.to raise_error /Invalid operation/
    end

    it 'cannot change its buffer while paused' do
      source.play
      source.pause
      expect { source.buffer = buffer }.to raise_error /Invalid operation/
    end

    it 'can change its buffer while stopped' do
      source.play
      source.stop
      expect { source.buffer = buffer }.to_not raise_error
    end

    it 'can change its buffer while in initial state' do
      expect { source.buffer = buffer }.to_not raise_error
    end

    it 'can share its buffer' do
      expect do
        another_source = Source.new
        another_source.buffer = source.buffer
        another_source.play
        source.play
      end.to_not raise_error
    end
  end

  context 'with a stream' do
    let(:another_stream) { Stream.new(WAV_PATH) }

    before(:each) { source.stream = stream }

    it 'can set the same stream to itself' do
      expect { source.stream = stream }.to_not raise_error
    end

    it 'cannot also have a buffer unless stream is detached' do
      error_pattern = /attach a buffer to a streaming source/
      expect { source.buffer = buffer }.to raise_error error_pattern
      source.play
      expect { source.buffer = buffer }.to raise_error error_pattern
      source.pause
      expect { source.buffer = buffer }.to raise_error error_pattern
      source.stop
      expect { source.buffer = buffer }.to raise_error error_pattern
      source.stream = nil
      expect { source.buffer = buffer }.to_not raise_error
    end

    it 'cannot have a closed stream' do
      another_stream.close
      expect { source.stream = another_stream }.to raise_error \
        /uninitialized stream/
    end

    it 'can only change its stream to one with compatible format' do
      expect { source.stream = another_stream }.to_not raise_error
      expect { source.stream = Stream.new(OV_PATH) }.to raise_error \
        /different audio format/
    end

    it 'can change to another stream in any state' do
      expect do
        source.stream = another_stream
        source.play
        source.stream = another_stream
        source.pause
        source.stream = another_stream
        source.stop
        source.stream = another_stream
      end.to_not raise_error
    end

    it 'can share its stream' do
      expect do
        another_source = Source.new
        another_source.stream = stream
        another_source.play
        source.play
        source.stream = another_stream
        another_source.stream = another_stream
      end.to_not raise_error
    end

    it 'fails to perform if stream is closed' do
      error_pattern = /uninitialized stream/
      stream.close
      expect { source.update }.to raise_error error_pattern
      expect { source.stop }.to raise_error error_pattern
      expect { source.play }.to raise_error error_pattern
    end
  end

  describe 'feeding effect slots' do
    it 'can only feed a limited number concurrently' do
      effect_slot = nil
      Seal.per_source_effect_limit.times do |i|
        effect_slot = EffectSlot.new
        source.feed(effect_slot, i)
      end
      expect { source.feed(effect_slot, Seal.per_source_effect_limit) }.to \
        raise_error /Invalid parameter value/
    end
  end

  describe 'automatic streaming' do
    let(:source) do
      Source.new.tap do |source|
        source.stream = Stream.new(OV_PATH)
        source.looping = true
        source.pitch = 3
      end
    end

    it 'continues in the background' do
      source.play
      sleep(0.5)
      expect(source.state).to be PLAYING
    end

    it 'can be disabled (taking effect upon the next play call)' do
      source.play
      source.auto = false
      sleep(0.5)
      # Not stopping streaming until the next play call.
      expect(source.state).to be PLAYING
      source.play
      sleep(0.5)
      expect(source.state).to be STOPPED
    end
  end

  describe 'looping' do
    example 'as undetermined type' do
      expect(source.looping).to be_falsey
      source.looping = true
      expect(source.looping).to be_truthy
      source.looping = false
      expect(source.looping).to be_falsey
    end

    example 'as streaming type' do
      source.looping = true
      source.stream = stream
      expect(source.looping).to be_truthy
      source.looping = false
      expect(source.looping).to be_falsey
      source.looping = true
      expect(source.looping).to be_truthy
    end

    example 'as static type' do
      source.looping = true
      source.buffer = buffer
      expect(source.looping).to be_truthy
      source.looping = false
      expect(source.looping).to be_falsey
      source.looping = true
      expect(source.looping).to be_truthy
    end

    # This example depends on the length of the test audio file.
    it 'is true if and only if source repeats' do
      source.stream = stream
      source.pitch = 2
      source.play
      sleep(0.3)
      expect(source.state).to be STOPPED
      source.looping = true
      source.play
      sleep(0.3)
      expect(source.state).to be PLAYING
    end
  end

  describe 'type' do
    it 'should initially be undetermined' do
      expect(source.type).to be UNDETERMINED
    end

    it 'should be streaming if it has a stream' do
      source.stream = stream
      expect(source.type).to be STREAMING
    end

    it 'should be static if it has a buffer' do
      source.buffer = buffer
      expect(source.type).to be STATIC
    end

    it 'should be undetermined after detaching' do
      source.stream = stream
      source.stream = nil
      expect(source.type).to be UNDETERMINED
      source.buffer = buffer
      source.buffer = nil
      expect(source.type).to be UNDETERMINED
    end
  end

  describe 'state transition' do
    before :each do
      source.tap { |source| source.stream = stream }
    end

    example 'from initial state' do
      expect(source.state).to be INITIAL
      source.stop
      expect(source.state).to be INITIAL
      source.pause
      expect(source.state).to be INITIAL
      source.rewind
      expect(source.state).to be INITIAL
      source.play
      expect(source.state).to be PLAYING
    end

    example 'from playing state' do
      source.play
      expect(source.state).to be PLAYING
      source.rewind
      expect(source.state).to be INITIAL
      source.play
      source.stop
      expect(source.state).to be STOPPED
      source.play
      source.pause
      expect(source.state).to be PAUSED
      source.play
      source.play
      expect(source.state).to be PLAYING
    end

    example 'from paused state' do
      source.play
      source.pause
      expect(source.state).to be PAUSED
      source.play
      expect(source.state).to be PLAYING
      source.pause
      source.rewind
      expect(source.state).to be INITIAL
      source.play
      source.pause
      source.stop
      expect(source.state).to be STOPPED
    end

    example 'from stopped state' do
      source.play
      source.stop
      expect(source.state).to be STOPPED
      source.pause
      expect(source.state).to be STOPPED
      source.play
      expect(source.state).to be PLAYING
      source.stop
      source.rewind
      expect(source.state).to be INITIAL
    end

    context 'when detaching' do
      example 'from initial state' do
        expect(source.state).to be INITIAL
        source.stream = nil
        expect(source.state).to be INITIAL
      end

      example 'from playing state' do
        source.play
        expect(source.state).to be PLAYING
        source.stream = nil
        expect(source.state).to be INITIAL
      end

      example 'from paused state' do
        source.play
        source.pause
        expect(source.state).to be PAUSED
        source.stream = nil
        expect(source.state).to be INITIAL
      end

      example 'from stopped state' do
        source.play
        source.stop
        expect(source.state).to be STOPPED
        source.stream = nil
        expect(source.state).to be INITIAL
      end
    end
  end
end
