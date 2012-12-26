require File.join(File.dirname(__FILE__), 'core')

module Seal
  class EffectSlot
    include Helper

    INIT = SealAPI.new('init_efs', 'p')
    DESTROY = SealAPI.new('destroy_efs', 'p')
    SET_EFFECT = SealAPI.new('set_efs_effect', 'pp')
    FEED = SealAPI.new('feed_efs', 'pip')
    SET_GAIN = SealAPI.new('set_efs_gain', 'pi')
    SET_AUTO = SealAPI.new('set_efs_auto', 'pi')
    GET_GAIN = SealAPI.new('get_efs_gain', 'pp')
    GET_AUTO = SealAPI.new('is_efs_auto', 'pp')

    def initialize(effect = nil)
      @effect_slot = '    '
      check_error(INIT[@effect_slot])
      self.effect = effect if effect
      ObjectSpace.define_finalizer(self, Helper.free(@effect_slot, DESTROY))
      self
    end

    def effect=(effect)
      native_effect_obj = effect ? effect.instance_variable_get(:@effect) : 0
      check_error(SET_EFFECT[@effect_slot, native_effect_obj])
      @effect = effect
      effect
    end

    attr_reader :effect

    def feed(index, source)
      native_source_obj = source.instance_variable_get(:@source)
      check_error(FEED[@effect_slot, index, native_source_obj])
      self
    end

    def gain=(gain)
      set_obj_float(@effect_slot, gain, SET_GAIN)
    end

    def gain
      get_obj_float(@effect_slot, GET_GAIN)
    end

    def auto=(auto)
      set_obj_char(@effect_slot, auto, SET_AUTO)
    end

    def auto
      get_obj_char(@effect_slot, GET_AUTO)
    end

    alias auto? auto
  end
end