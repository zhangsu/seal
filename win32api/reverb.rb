module Seal
  class Reverb
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