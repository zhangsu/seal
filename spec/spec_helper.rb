require 'rspec/its'

Dir["./spec/support/**/*.rb"].each { |f| require f }
require 'seal'
include Seal

TOLERANCE = 0.00001
FIXTURE_DIR = File.join File.dirname(__FILE__), 'fixtures'
WAV_PATH = File.join FIXTURE_DIR, 'tone_up.wav'
OV_PATH = File.join FIXTURE_DIR, 'heal.ogg'

RSpec.configure do |config|

  config.instance_eval do
    include CustomMatchers
    include Helpers, :include_helpers

    before :all do
      Seal.startup
    end

    after :all do
      Seal.cleanup
    end
  end
end
