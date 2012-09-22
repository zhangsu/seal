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

module Helpers
  def fresh_start
    before :all do
      Seal.startup
    end

    after :all do
      Seal.cleanup
    end
  end
end

require 'seal'
include Seal

TOLERANCE = 0.00001
FIXTURE_DIR = File.join File.dirname(__FILE__), 'fixtures'
WAV_PATH = File.join FIXTURE_DIR, 'tone_up.wav'
OV_PATH = File.join FIXTURE_DIR, 'heal.ogg'

RSpec.configure do |config|
  include Helpers
  config.include CustomMatchers
  config.include Helpers

  config.instance_eval &:fresh_start
end
