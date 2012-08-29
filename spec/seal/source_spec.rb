require 'spec_helper'

describe Source do
  describe 'default attributes' do
    let(:source) { Source.new }

    it 'is automatic' do
      source.auto?.should be_true
    end

    it 'has no buffer' do
      source.buffer.should be_nil
    end

    it 'has a chunk size of 36864' do
      source.chunk_size.should == 36864
    end

    it 'has a gain of 1.0' do
      source.gain.should be_within(TOLERANCE).of(1.0)
    end

    it 'is not looping' do
      source.looping.should be_false
    end

    it 'has a pitch of 1.0' do
      source.pitch.should be_within(TOLERANCE).of(1.0)
    end

    it 'has a position of (0,0,0)' do
      source.position.should == [0, 0, 0]
    end

    it 'has a queue size of 3' do
      source.queue_size.should == 3
    end

    it 'is not relative' do
      source.relative.should be_false
    end

    it 'is in the initial state' do
      source.state.should == Source::State::INITIAL
    end

    it 'has no stream' do
      source.stream.should be_nil
    end

    it 'has un undertermined type' do
      source.type.should == Source::Type::UNDETERMINED
    end

    it 'has a velocity of (0,0,0)' do
      source.velocity.should == [0, 0, 0]
    end
  end
end
