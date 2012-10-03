require 'spec_helper'

describe Listener do
  subject { LISTENER }

  it_behaves_like 'a movable object'

  describe 'by default' do
    its(:gain) { should eq 1.0 }
    its(:orientation) { should eq [[0, 0, -1], [0, 1, 0]] }
  end

  it 'can change its orientation' do
    old_orientation = LISTENER.orientation
    LISTENER.orientation = [0, 1, 0], [3, 0, 0]
    LISTENER.orientation[0].should eq [0, 1, 0]
    LISTENER.orientation[1].should eq [3, 0, 0]
    LISTENER.orientation = old_orientation
  end

  it "validates the float attribute 'gain' is in [0, +inf.)" do
    old_gain = LISTENER.gain
    LISTENER.gain = 0.34767
    LISTENER.gain.should be_within(TOLERANCE).of 0.34767
    LISTENER.gain = 10
    error_pattern = /Invalid parameter value/
    expect { LISTENER.gain = -0.1 }.to raise_error error_pattern
    expect { LISTENER.gain = -1.3 }.to raise_error error_pattern
    expect { LISTENER.gain = -3203 }.to raise_error error_pattern
    LISTENER.gain.should eq 10
    LISTENER.gain = old_gain
  end
end
