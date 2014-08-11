RSpec.configure do |config|
  config.instance_eval do
    alias_it_should_behave_like_to :it_validates, 'validates that'
    alias_it_should_behave_like_to :it_defines, 'defines'
  end
end
