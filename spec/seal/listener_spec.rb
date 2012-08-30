require 'spec_helper'

describe Listener do
  describe 'default attributes' do
    fresh_start

    it 'has a default position of (0,0,0)' do
      LISTENER.position.should == [0, 0, 0]
    end

    it 'has a default velocity of (0,0,0)' do
      LISTENER.position.should == [0, 0, 0]
    end

    it 'has a default orientation of ((0,0,-1),(0,1,0))' do
      LISTENER.orientation.should == [[0, 0, -1], [0, 1, 0]]
    end

    it 'has a default gain of 1.0' do
      LISTENER.gain.should == 1.0
    end
  end

  it 'sets and gets position' do
    LISTENER.position = 3.2, 3.1, -0.3
    LISTENER.position.should be_each_within(TOLERANCE).of [3.2, 3.1, -0.3]
  end

  it 'sets and gets velocity' do
    LISTENER.velocity = 1.3, -3, 0.566
    LISTENER.velocity.should be_each_within(TOLERANCE).of [1.3, -3, 0.566]
  end

  it 'sets and gets orientation' do
    LISTENER.orientation = [0, 1, 0], [3, 0, 0]
    LISTENER.orientation[0].should == [0, 1, 0]
    LISTENER.orientation[1].should == [3, 0, 0]
  end

  it 'can set valid gain and get gain' do
    LISTENER.gain = 0.34767
    LISTENER.gain.should be_within(TOLERANCE).of 0.34767
    LISTENER.gain = 10
    expect { LISTENER.gain = -0.1 }.to raise_error SealError
    expect { LISTENER.gain = -1.3 }.to raise_error SealError
    expect { LISTENER.gain = -3203 }.to raise_error SealError
    LISTENER.gain.should == 10
  end
end
