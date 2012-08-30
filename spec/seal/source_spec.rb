require 'spec_helper'

describe Source do
  describe 'default attributes' do
    before :all do
      @source = Source.new
    end

    it 'is automatic' do
      @source.auto?.should be_true
    end

    it 'has no buffer' do
      @source.buffer.should be_nil
    end

    it 'has a chunk size of 36864' do
      @source.chunk_size.should == 36864
    end

    it 'has a gain of 1.0' do
      @source.gain.should be_within(TOLERANCE).of(1.0)
    end

    it 'is not looping' do
      @source.looping.should be_false
    end

    it 'has a pitch of 1.0' do
      @source.pitch.should be_within(TOLERANCE).of(1.0)
    end

    it 'has a position of (0,0,0)' do
      @source.position.should == [0, 0, 0]
    end

    it 'has a queue size of 3' do
      @source.queue_size.should == 3
    end

    it 'is not relative' do
      @source.relative.should be_false
    end

    it 'is in the initial state' do
      @source.state.should == Source::State::INITIAL
    end

    it 'has no stream' do
      @source.stream.should be_nil
    end

    it 'has un undertermined type' do
      @source.type.should == Source::Type::UNDETERMINED
    end

    it 'has a velocity of (0,0,0)' do
      @source.velocity.should == [0, 0, 0]
    end
  end

  describe 'attributes setting and validity' do
    before :all do
      @source = Source.new
    end

    it 'can have a queue size in [2, 63]' do
      expect { @source.queue_size = -130 }.to raise_error SealError
      expect { @source.queue_size = 0 }.to raise_error SealError
      expect { @source.queue_size = 1 }.to raise_error SealError
      @source.queue_size = 2
      @source.queue_size.should == 2
      @source.queue_size = 32
      @source.queue_size.should == 32
      @source.queue_size = 63
      @source.queue_size.should == 63
      expect { @source.queue_size = 64 }.to raise_error SealError
      expect { @source.queue_size = 1203 }.to raise_error SealError
      @source.queue_size.should == 63
    end

    it 'can have a auto-adjusting chunk size in [9216, 16773120]' do
      expect { @source.chunk_size = 0 }.to raise_error SealError
      expect { @source.chunk_size = 432 }.to raise_error SealError
      expect { @source.chunk_size = 9215 }.to raise_error SealError
      @source.chunk_size = 9216
      @source.chunk_size.should == 9216
      # It should be automatically adjusted to a smaller multiple of 9216.
      @source.chunk_size = 9217
      @source.chunk_size.should == 9216
      @source.chunk_size = 32768
      @source.chunk_size.should == 27648
      @source.chunk_size = 294912
      @source.chunk_size.should == 294912
      @source.chunk_size = 16773119
      @source.chunk_size.should == 16763904
      @source.chunk_size = 16773120
      @source.chunk_size.should == 16773120
      expect { @source.chunk_size = 16773121 }.to raise_error SealError
      expect { @source.chunk_size = 234923428 }.to raise_error SealError
    end
  end
end
