require 'spec_helper'

describe Seal do
  it 'should initialize and finalize correctly on default device' do
    Seal.startup
    Seal.cleanup
  end

  it 'should fail when trying to initialze non-existing device' do
    expect { Seal.startup 'foo42' }.to raise_error(SealError)
  end

  it 'has only one instance of Listener' do
    Seal.listener.should == Seal.listener
    Seal.listener.is_a?(Listener).should be_true
    expect { Seal::Listener.new }.to raise_error(NoMethodError)
    expect { Seal::Listener.allocate }.to raise_error(TypeError)
  end

  it 'defines a limit on the number of effect slots per source' do
    Seal.per_source_effect_limit.is_a?(Integer).should be_true
  end

  it 'defines a version string' do
    Seal::VERSION.is_a?(String).should be_true
  end
end
