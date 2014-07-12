require 'spec_helper'

shared_examples 'a movable object' do
  describe 'by default' do
    its(:position) { is_expected.to eq [0, 0, 0] }
    its(:velocity) { is_expected.to eq [0, 0, 0] }
  end

  context 'with motion', :include_helpers do
    it 'changes its position' do
      with_motion(subject) do
        subject.position = 3.2, 3.1, -0.3
        expect(subject.position).to be_each_within(TOLERANCE)
          .of [3.2, 3.1, -0.3]
      end
    end

    it 'changes its velocity' do
      with_motion(subject) do
        subject.velocity = 1.3, -3, 0.566
        expect(subject.velocity).to be_each_within(TOLERANCE)
          .of [1.3, -3, 0.566]
      end
    end

    it 'moves based on velocity' do
      with_motion(subject) do
        subject.velocity = 3, -3.51, 0.2
        5.times do
          position = subject.position
          new_position = position.zip(subject.velocity).map { |x, y| x + y }
          subject.move
          expect(subject.position).to be_each_within(TOLERANCE).of new_position
        end
      end
    end
  end
end
