guard 'rspec' do
  watch(%r{^spec/.+_spec\.rb$})
  watch(%r{^lib/(.+)\.so$})     { |m| "spec" }
  watch('spec/spec_helper.rb')  { "spec" }
end

