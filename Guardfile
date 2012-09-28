guard 'rspec', cli: '--color --format doc' do
  watch(%r{^spec/.+_spec\.rb$})
  watch(%r{^spec/.+(?<!_spec).rb$}) { 'spec' }
  watch(%r{^lib/.+\.so$})     { 'spec' }
end

