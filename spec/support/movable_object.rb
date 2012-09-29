require 'spec_helper'

shared_examples 'a movable object' do
  describe 'by default' do
    its(:position) { should eq [0, 0, 0] }
    its(:velocity) { should eq [0, 0, 0] }
  end

  it 'can change its position' do
    old_position = subject.position
    subject.position = 3.2, 3.1, -0.3
    subject.position.should be_each_within(TOLERANCE).of [3.2, 3.1, -0.3]
    subject.position = old_position
  end

  it 'can change its velocity' do
    old_velocity = subject.velocity
    subject.velocity = 1.3, -3, 0.566
    subject.velocity.should be_each_within(TOLERANCE).of [1.3, -3, 0.566]
    subject.velocity = old_velocity
  end
end
