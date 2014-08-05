module CustomMatchers
  class BeEachWithin < RSpec::Matchers::BuiltIn::BeWithin
    def matches?(target)
      target.zip(@expected).all? { |a, b| (a - b).abs <= @delta }
    end

    def description
      "be each within #{@delta} of the parallel elements in #{@expected}"
    end
  end

  def be_each_within(tolerance)
    BeEachWithin.new(tolerance)
  end
end

class Symbol
  def to_writer
    "#{self}=".intern
  end
end
