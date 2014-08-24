require 'spec_helper'

shared_examples 'the boolean attribute' do |reader_sym|
  writer_sym = reader_sym.to_writer

  example "'#{reader_sym}' can be true" do
    subject.send writer_sym, true
    expect(subject.send(reader_sym)).to be_truthy
  end

  example "'#{reader_sym}' can be false" do
    subject.send writer_sym, false
    expect(subject.send(reader_sym)).to be_falsey
  end

  example "'#{reader_sym}' can be truthy" do
    subject.send writer_sym, Object.new
    expect(subject.send(reader_sym)).to be_truthy
  end

  example "'#{reader_sym}' can be falsey" do
    subject.send writer_sym, nil
    expect(subject.send(reader_sym)).to be_falsey
  end
end

shared_examples 'the bounded float attribute' do |reader_sym, interval|
  writer_sym = reader_sym.to_writer
  interval =~ /(\[|\()\s*(.+?)\s*,\s*(.+?)\s*(\]|\))/
  lower = $2 == '-inf.' ? -Float::INFINITY : $2.to_f
  upper = $3 == '+inf.' ? +Float::INFINITY : $3.to_f
  # Whether the boundaries are open.
  lopen = $1 == '('
  uopen = $4 == ')'
  error_pattern = /Invalid parameter value/

  specify "'#{reader_sym}' is in #$1#$2,#$3#$4" do
    if lower != -Float::INFINITY
      # Validates values smaller than the lower bound.
      [lower - 4096, lower - 0.01].each do |value|
        expect { subject.send writer_sym, value }.to raise_error error_pattern
      end

      # Validates the lower bound.
      if lopen
        expect { subject.send writer_sym, lower }.to raise_error error_pattern
      else
        subject.send writer_sym, lower
        expect(subject.send(reader_sym)).to be_within(TOLERANCE).of lower
      end
    end

    # Validates values within the bounds.
    l, u =
      if lower == -Float::INFINITY and upper == Float::INFINITY
        [-4096, 4096]
      elsif lower == -Float::INFINITY
        [upper - 4096, upper]
      elsif upper == Float::INFINITY
        [lower, lower + 4096]
      else
        [lower, upper]
      end
    [l + 0.01, (l + u) / 2.0, u - 0.01].each do |value|
      subject.send writer_sym, value
      expect(subject.send(reader_sym)).to be_within(TOLERANCE).of(value)
    end

    if upper != Float::INFINITY
      # Validates the upper bound.
      if uopen
        expect { subject.send writer_sym, upper }.to raise_error error_pattern
      else
        subject.send writer_sym, upper
        expect(subject.send(reader_sym)).to be_within(TOLERANCE).of upper
      end

      # Validates values greater than the upper bound.
      [upper + 0.01, upper + upper.abs].each do |value|
        expect { subject.send writer_sym, value }.to raise_error error_pattern
      end
    end
  end
end
