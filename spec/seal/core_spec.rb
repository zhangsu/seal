require 'spec_helper'

describe Seal do
  it 'should initialize and finalize correctly on default device' do
    Seal.startup
    Seal.cleanup
  end

  it 'should fail when trying to initialze non-existing device' do
    expect { Seal.startup 'foo42' }.to raise_error(SealError)
  end
end
