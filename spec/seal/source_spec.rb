require 'spec_helper'

include Source::State
include Source::Type

describe Source do
  let(:source) { Source.new }
  let(:stream) { Stream.new(WAV_PATH) }
  let(:buffer) { Buffer.new(WAV_PATH) }

  it_behaves_like 'a movable object'

  describe 'by default' do
    its(:auto) { should be_true }
    its(:buffer) { should be_nil }
    its(:chunk_size) { should eq 36864 }
    its(:gain) { should be_within(TOLERANCE).of(1.0) }
    its(:looping) { should be_false }
    its(:pitch) { should be_within(TOLERANCE).of(1.0) }
    its(:queue_size) { should eq 3 }
    its(:relative) { should be_false }
    its(:state) { should be INITIAL }
    its(:stream) { should be_nil }
    its(:type) { should be UNDETERMINED }
  end

  describe 'instance method aliases' do
    [:auto, :relative, :looping].each do |name|
      specify "#{name}? is an alias of #{name}" do
        subject.method(name).should eq subject.method("#{name}?")
      end
    end
  end

  it_validates 'the boolean attribute', :relative
  it_validates 'the boolean attribute', :auto
  it_validates 'the float attribute', :pitch, "[0, +inf.)"
  it_validates 'the float attribute', :gain, "[0, +inf.)"

  it 'validates its queue size is in [2, 63]' do
    error_pattern = /Invalid parameter value/
    expect { source.queue_size = -130 }.to raise_error error_pattern
    expect { source.queue_size = 0 }.to raise_error error_pattern
    expect { source.queue_size = 1 }.to raise_error error_pattern
    source.queue_size = 2
    source.queue_size.should eq 2
    source.queue_size = 32
    source.queue_size.should eq 32
    source.queue_size = 63
    source.queue_size.should eq 63
    expect { source.queue_size = 64 }.to raise_error error_pattern
    expect { source.queue_size = 1203 }.to raise_error error_pattern
    source.queue_size.should eq 63
  end

  it 'validates it has an auto-adjusting chunk size in [9216, 16773120]' do
    error_pattern = /Invalid parameter value/
    expect { source.chunk_size = 0 }.to raise_error error_pattern
    expect { source.chunk_size = 432 }.to raise_error error_pattern
    expect { source.chunk_size = 9215 }.to raise_error error_pattern
    source.chunk_size = 9216
    source.chunk_size.should eq 9216
    # It should be automatically adjusted to a smaller multiple of 9216.
    source.chunk_size = 9217
    source.chunk_size.should eq 9216
    source.chunk_size = 32768
    source.chunk_size.should eq 27648
    source.chunk_size = 294912
    source.chunk_size.should eq 294912
    source.chunk_size = 16773119
    source.chunk_size.should eq 16763904
    source.chunk_size = 16773120
    source.chunk_size.should eq 16773120
    expect { source.chunk_size = 16773121 }.to raise_error error_pattern
    expect { source.chunk_size = 234923428 }.to raise_error error_pattern
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
      source.state.should be PLAYING
    end

    it 'can be disabled (taking effect upon the next play call)' do
      source.play
      source.auto = false
      sleep(0.5)
      # Not stopping streaming until the next play call.
      source.state.should be PLAYING
      source.play
      sleep(0.5)
      source.state.should be STOPPED
    end
  end

  describe 'looping' do
    example 'as undetermined type' do
      source.looping.should be_false
      source.looping = true
      source.looping.should be_true
      source.looping = false
      source.looping.should be_false
    end

    example 'as streaming type' do
      source.looping = true
      source.stream = stream
      source.looping.should be_true
      source.looping = false
      source.looping.should be_false
      source.looping = true
      source.looping.should be_true
    end

    example 'as static type' do
      source.looping = true
      source.buffer = buffer
      source.looping.should be_true
      source.looping = false
      source.looping.should be_false
      source.looping = true
      source.looping.should be_true
    end

    # This example depends on the length of the test audio file.
    it 'is true if and only if source repeats' do
      source.stream = stream
      source.pitch = 2
      source.play
      sleep(0.3)
      source.state.should be STOPPED
      source.looping = true
      source.play
      sleep(0.3)
      source.state.should be PLAYING
    end
  end

  describe 'type' do
    it 'should initially be undetermined' do
      source.type.should be UNDETERMINED
    end

    it 'should be streaming if it has a stream' do
      source.stream = stream
      source.type.should be STREAMING
    end

    it 'should be static if it has a buffer' do
      source.buffer = buffer
      source.type.should be STATIC
    end

    it 'should be undetermined after detaching' do
      source.stream = stream
      source.stream = nil
      source.type.should be UNDETERMINED
      source.buffer = buffer
      source.buffer = nil
      source.type.should be UNDETERMINED
    end
  end

  describe 'state transition' do
    before :each do
      source.tap { |source| source.stream = stream }
    end

    example 'from initial state' do
      source.state.should be INITIAL
      source.stop
      source.state.should be INITIAL
      source.pause
      source.state.should be INITIAL
      source.rewind
      source.state.should be INITIAL
      source.play
      source.state.should be PLAYING
    end

    example 'from playing state' do
      source.play
      source.state.should be PLAYING
      source.rewind
      source.state.should be INITIAL
      source.play
      source.stop
      source.state.should be STOPPED
      source.play
      source.pause
      source.state.should be PAUSED
      source.play
      source.play
      source.state.should be PLAYING
    end

    example 'from paused state' do
      source.play
      source.pause
      source.state.should be PAUSED
      source.play
      source.state.should be PLAYING
      source.pause
      source.rewind
      source.state.should be INITIAL
      source.play
      source.pause
      source.stop
      source.state.should be STOPPED
    end

    example 'from stopped state' do
      source.play
      source.stop
      source.state.should be STOPPED
      source.pause
      source.state.should be STOPPED
      source.play
      source.state.should be PLAYING
      source.stop
      source.rewind
      source.state.should be INITIAL
    end

    context 'when detaching' do
      example 'from initial state' do
        source.state.should be INITIAL
        source.stream = nil
        source.state.should be INITIAL
      end

      example 'from playing state' do
        source.play
        source.state.should be PLAYING
        source.stream = nil
        source.state.should be INITIAL
      end

      example 'from paused state' do
        source.play
        source.pause
        source.state.should be PAUSED
        source.stream = nil
        source.state.should be INITIAL
      end

      example 'from stopped state' do
        source.play
        source.stop
        source.state.should be STOPPED
        source.stream = nil
        source.state.should be INITIAL
      end
    end
  end
end
