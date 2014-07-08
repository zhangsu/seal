require 'spec_helper'

shared_examples 'boolean reader aliases' do |method_names|
  method_names.each do |name|
    specify "#{name}? is an alias of #{name}" do
      expect(subject.method(name)).to eq subject.method("#{name}?")
    end
  end
end
