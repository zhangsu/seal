module Helpers
  def with_motion(movable)
    original_position = movable.position
    original_velocity = movable.velocity
    yield
    movable.position = original_position
    movable.velocity = original_velocity
  end
end
