require 'spec_helper'

describe Seal do
  it 'should initialize and finalize correctly on default device' do
    expect do
      Seal.startup
      Seal.cleanup
    end.to_not raise_error
  end

  it 'should fail when trying to initialize non-existing device' do
    expect { Seal.startup 'foo42' }.to raise_error SealError
  end

  it 'has only one instance of Listener' do
    Seal.listener.should be Seal.listener
    Seal.listener.should be_a Listener
    expect { Seal::Listener.new }.to raise_error NoMethodError
    expect { Seal::Listener.allocate }.to raise_error TypeError
  end

  it 'defines a limit on the number of effect slots per source' do
    Seal.per_source_effect_limit.should be_an Integer
  end

  it 'defines a version string' do
    Seal::VERSION.should match /\d\.\d\.\d/
  end
end
