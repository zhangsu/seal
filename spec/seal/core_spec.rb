require 'spec_helper'

describe Seal do
  context 'initially' do
    # Examples in this group needs to call Seal.startup themselves instead of
    # relying on it being implicitly called by the global before hook.
    # Therefore, Seal must be finalized in order to avoid double startup, which
    # may fail for some devices due to opening an already opened, busy device.
    before { Seal.cleanup }
    # Since there is a global after hook for Seal.cleanup, we insert this after
    # hook before the global one so the global one will then cleans up
    # properly.
    prepend_after { Seal.startup }

    it 'starts up and cleans up properly on default device' do
      expect do
        Seal.startup
        Seal.cleanup
      end.to_not raise_error
    end

    it 'fails when trying to start up on non-existing device' do
      expect { Seal.startup 'foo42' }.to raise_error SealError
    end
  end

  it 'has a pre-allocated Listener instance' do
    expect(Seal.listener).to be_a Listener
  end

  it 'can only have one instance of Listener' do
    expect { Seal::Listener.new }.to raise_error NoMethodError
    expect { Seal::Listener.allocate }.to raise_error
  end

  it 'defines a limit on the number of effect slots per source' do
    expect(Seal.per_source_effect_limit).to be_an Integer
  end

  it 'defines a version string' do
    expect(Seal::VERSION).to match /\d\.\d\.\d/
  end
end
