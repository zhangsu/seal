require File.join(File.dirname(__FILE__), 'core')

module Seal
  class Reverb
    include Helper

    INIT = SealAPI.new('init_rvb', 'p')
    DESTROY = SealAPI.new('destroy_rvb', 'p')
    LOAD = SealAPI.new('load_rvb', 'pi')
    SET_DENSITY = SealAPI.new('set_rvb_density', 'pi')
    SET_DIFFUSION = SealAPI.new('set_rvb_diffusion', 'pi')
    SET_GAIN = SealAPI.new('set_rvb_gain', 'pi')
    SET_HFGAIN = SealAPI.new('set_rvb_hfgain', 'pi')
    SET_DECAY_TIME = SealAPI.new('set_rvb_decay_time', 'pi')
    SET_HFDECAY_RATIO = SealAPI.new('set_rvb_hfdecay_ratio', 'pi')
    SET_REFLECTIONS_GAIN = SealAPI.new('set_rvb_reflections_gain', 'pi')
    SET_REFLECTIONS_DELAY = SealAPI.new('set_rvb_reflections_delay', 'pi')
    SET_LATE_GAIN = SealAPI.new('set_rvb_late_gain', 'pi')
    SET_LATE_DELAY = SealAPI.new('set_rvb_late_delay', 'pi')
    SET_AIR_ABSORBTION_HFGAIN =
      SealAPI.new('set_rvb_air_absorbtion_hfgain', 'pi')
    SET_ROOM_ROLLOFF_FACTOR = SealAPI.new('set_rvb_room_rolloff_factor', 'pi')
    SET_HFDECAY_LIMITED = SealAPI.new('set_rvb_hfdecay_limited', 'pi')
    GET_DENSITY = SealAPI.new('get_rvb_density', 'pp')
    GET_DIFFUSION = SealAPI.new('get_rvb_diffusion', 'pp')
    GET_GAIN = SealAPI.new('get_rvb_gain', 'pp')
    GET_HFGAIN = SealAPI.new('get_rvb_hfgain', 'pp')
    GET_DECAY_TIME = SealAPI.new('get_rvb_decay_time', 'pp')
    GET_HFDECAY_RATIO = SealAPI.new('get_rvb_hfdecay_ratio', 'pp')
    GET_REFLECTIONS_GAIN = SealAPI.new('get_rvb_reflections_gain', 'pp')
    GET_REFLECTIONS_DELAY = SealAPI.new('get_rvb_reflections_delay', 'pp')
    GET_LATE_GAIN = SealAPI.new('get_rvb_late_gain', 'pp')
    GET_LATE_DELAY = SealAPI.new('get_rvb_late_delay', 'pp')
    GET_AIR_ABSORBTION_HFGAIN =
      SealAPI.new('get_rvb_air_absorbtion_hfgain', 'pp')
    GET_ROOM_ROLLOFF_FACTOR = SealAPI.new('get_rvb_room_rolloff_factor', 'pp')
    IS_HFDECAY_LIMITED = SealAPI.new('is_rvb_hfdecay_limited', 'pp')

    def initialize(preset = nil)
      @reverb = '    '
      check_error(INIT[@reverb])
      load(preset) if preset
      ObjectSpace.define_finalizer(self, Helper.free(@reverb, DESTROY))
    end

    def load(preset)
      check_error(LOAD[@reverb, preset])
    end

    def density=(density)
      set_obj_float(@reverb, density, SET_DENSITY)
    end

    def diffusion=(diffusion)
      set_obj_float(@reverb, diffusion, SET_DIFFUSION)
    end

    def gain=(gain)
      set_obj_float(@reverb, gain, SET_GAIN)
    end

    def hfgain=(hfgain)
      set_obj_float(@reverb, hfgain, SET_HFGAIN)
    end

    def decay_time=(decay_time)
      set_obj_float(@reverb, decay_time, SET_DECAY_TIME)
    end

    def hfdecay_ratio=(hfdecay_ratio)
      set_obj_float(@reverb, hfdecay_ratio, SET_HFDECAY_RATIO)
    end

    def reflections_gain=(reflections_gain)
      set_obj_float(@reverb, reflections_gain, SET_REFLECTIONS_GAIN)
    end

    def reflections_delay=(reflections_delay)
      set_obj_float(@reverb, reflections_delay, SET_REFLECTIONS_DELAY)
    end

    def late_gain=(late_gain)
      set_obj_float(@reverb, late_gain, SET_LATE_GAIN)
    end

    def late_delay=(late_delay)
      set_obj_float(@reverb, late_delay, SET_LATE_DELAY)
    end

    def air_absorbtion_hfgain=(air_absorbtion_hfgain)
      set_obj_float(@reverb, air_absorbtion_hfgain, SET_AIR_ABSORBTION_HFGAIN)
    end

    def room_rolloff_factor=(room_rolloff_factor)
      set_obj_float(@reverb, room_rolloff_factor, SET_ROOM_ROLLOFF_FACTOR)
    end

    def hfdecay_limited=(hfdecay_limited)
      set_obj_char(@reverb, hfdecay_limited, SET_HFDECAY_LIMITED)
    end

    def density
      get_obj_float(@reverb, GET_DENSITY)
    end

    def diffusion
      get_obj_float(@reverb, GET_DIFFUSION)
    end

    def gain
      get_obj_float(@reverb, GET_GAIN)
    end

    def hfgain
      get_obj_float(@reverb, GET_HFGAIN)
    end

    def decay_time
      get_obj_float(@reverb, GET_DECAY_TIME)
    end

    def hfdecay_ratio
      get_obj_float(@reverb, GET_HFDECAY_RATIO)
    end

    def reflections_gain
      get_obj_float(@reverb, GET_REFLECTIONS_GAIN)
    end

    def reflections_delay
      get_obj_float(@reverb, GET_REFLECTIONS_DELAY)
    end

    def late_gain
      get_obj_float(@reverb, GET_LATE_GAIN)
    end

    def late_delay
      get_obj_float(@reverb, GET_LATE_DELAY)
    end

    def air_absorbtion_hfgain
      get_obj_float(@reverb, GET_AIR_ABSORBTION_HFGAIN)
    end

    def room_rolloff_factor
      get_obj_float(@reverb, GET_ROOM_ROLLOFF_FACTOR)
    end

    def hfdecay_limited
      get_obj_char(@reverb, IS_HFDECAY_LIMITED)
    end

    alias hfdecay_limited? hfdecay_limited

    module Preset
      Helper.define_enum(self, [
        :GENERIC,
        :PADDEDCELL,
        :ROOM,
        :BATHROOM,
        :LIVINGROOM,
        :STONEROOM,
        :AUDITORIUM,
        :CONCERTHALL,
        :CAVE,
        :ARENA,
        :HANGAR,
        :CARPETEDHALLWAY,
        :HALLWAY,
        :STONECORRIDOR,
        :ALLEY,
        :FOREST,
        :CITY,
        :MOUNTAINS,
        :QUARRY,
        :PLAIN,
        :PARKINGLOT,
        :SEWERPIPE,
        :UNDERWATER,
        :DRUGGED,
        :DIZZY,
        :PSYCHOTIC
      ])

      module Castle
        Helper.define_enum(self, [
          :SMALLROOM,
          :SHORTPASSAGE,
          :MEDIUMROOM,
          :LARGEROOM,
          :LONGPASSAGE,
          :HALL,
          :CUPBOARD,
          :COURTYARD,
          :ALCOVE
        ], Preset::PSYCHOTIC + 1)
      end

      module Factory
        Helper.define_enum(self, [
          :SMALLROOM,
          :SHORTPASSAGE,
          :MEDIUMROOM,
          :LARGEROOM,
          :LONGPASSAGE,
          :HALL,
          :CUPBOARD,
          :COURTYARD,
          :ALCOVE
        ], Castle::ALCOVE + 1)
      end

      module IcePalace
        Helper.define_enum(self, [
          :SMALLROOM,
          :SHORTPASSAGE,
          :MEDIUMROOM,
          :LARGEROOM,
          :LONGPASSAGE,
          :HALL,
          :CUPBOARD,
          :COURTYARD,
          :ALCOVE
        ], Factory::ALCOVE + 1)
      end

      module SpaceStation
        Helper.define_enum(self, [
          :SMALLROOM,
          :SHORTPASSAGE,
          :MEDIUMROOM,
          :LARGEROOM,
          :LONGPASSAGE,
          :HALL,
          :CUPBOARD,
          :ALCOVE
        ], IcePalace::ALCOVE + 1)
      end

      module WoodenGalleon
        Helper.define_enum(self, [
          :SMALLROOM,
          :SHORTPASSAGE,
          :MEDIUMROOM,
          :LARGEROOM,
          :LONGPASSAGE,
          :HALL,
          :CUPBOARD,
          :COURTYARD,
          :ALCOVE
        ], SpaceStation::ALCOVE + 1)
      end

      module Sports
        Helper.define_enum(self, [
          :EMPTYSTADIUM,
          :SQUASHCOURT,
          :SMALLSWIMMINGPOOL,
          :LARGESWIMMINGPOOL,
          :GYMNASIUM,
          :FULLSTADIUM,
          :STADIUMTANNOY
        ], WoodenGalleon::ALCOVE + 1)
      end

      module Prefab
        Helper.define_enum(self, [
          :WORKSHOP,
          :SCHOOLROOM,
          :PRACTISEROOM,
          :OUTHOUSE,
          :CARAVAN
        ], Sports::STADIUMTANNOY + 1)
      end

      module Dome
        Helper.define_enum(self, [
          :TOMB,
          :SAINTPAULS
        ], Prefab::CARAVAN + 1)
      end

      module Pipe
        Helper.define_enum(self, [
          :SMALL,
          :LONGTHIN,
          :LARGE,
          :RESONANT
        ], Dome::SAINTPAULS + 1)
      end

      module Outdoors
        Helper.define_enum(self, [
          :BACKYARD,
          :ROLLINGPLAINS,
          :DEEPCANYON,
          :CREEK,
          :VALLEY
        ], Pipe::RESONANT + 1)
      end

      module Mood
        Helper.define_enum(self, [
          :HEAVEN,
          :HELL,
          :MEMORY
        ], Outdoors::VALLEY + 1)
      end

      module Driving
        Helper.define_enum(self, [
          :COMMENTATOR,
          :PITGARAGE,
          :INCAR_RACER,
          :INCAR_SPORTS,
          :INCAR_LUXURY,
          :FULLGRANDSTAND,
          :EMPTYGRANDSTAND,
          :TUNNEL
        ], Mood::MEMORY + 1)
      end

      module City
        Helper.define_enum(self, [
          :STREETS,
          :SUBWAY,
          :MUSEUM,
          :LIBRARY,
          :UNDERPASS,
          :ABANDONED
        ], Driving::TUNNEL + 1)
      end

      module Misc
        Helper.define_enum(self, [
          :DUSTYROOM,
          :CHAPEL,
          :SMALLWATERROOM
        ], City::ABANDONED + 1)
      end
    end
  end
end