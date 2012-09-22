require 'spec_helper'

describe Listener do
  describe 'by default' do
    it 'has a default position of (0,0,0)' do
      LISTENER.position.should eq [0, 0, 0]
    end

    it 'has a default velocity of (0,0,0)' do
      LISTENER.velocity.should eq [0, 0, 0]
    end

    it 'has a default orientation of ((0,0,-1),(0,1,0))' do
      LISTENER.orientation.should eq [[0, 0, -1], [0, 1, 0]]
    end

    it 'has a default gain of 1.0' do
      LISTENER.gain.should eq 1.0
    end
  end

  describe 'with valid attributes' do
    it 'can change its position' do
      old_position = LISTENER.position
      LISTENER.position = 3.2, 3.1, -0.3
      LISTENER.position.should be_each_within(TOLERANCE).of [3.2, 3.1, -0.3]
      LISTENER.position = old_position
    end

    it 'can change its velocity' do
      old_velocity = LISTENER.velocity
      LISTENER.velocity = 1.3, -3, 0.566
      LISTENER.velocity.should be_each_within(TOLERANCE).of [1.3, -3, 0.566]
      LISTENER.velocity = old_velocity
    end

    it 'can change its orientation' do
      old_orientation = LISTENER.orientation
      LISTENER.orientation = [0, 1, 0], [3, 0, 0]
      LISTENER.orientation[0].should eq [0, 1, 0]
      LISTENER.orientation[1].should eq [3, 0, 0]
      LISTENER.orientation = old_orientation
    end

    it 'has a gain in [0, +inf.)' do
      old_gain = LISTENER.gain
      LISTENER.gain = 0.34767
      LISTENER.gain.should be_within(TOLERANCE).of 0.34767
      LISTENER.gain = 10
      expect { LISTENER.gain = -0.1 }.to raise_error SealError
      expect { LISTENER.gain = -1.3 }.to raise_error SealError
      expect { LISTENER.gain = -3203 }.to raise_error SealError
      LISTENER.gain.should eq 10
      LISTENER.gain = old_gain
    end
  end
end
