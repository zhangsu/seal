#include <string.h>

#include "test.h"
#include "seal.h"

void
test_listener_attr(void)
{
    float pos[] = { 30.2f, 435.3f, -4.5f };
    float actual_pos[] = { 0.0f, 0.0f, 0.0f };
    float default_pos[] = { 0.0f, 0.0f, 0.0f };
    float vel[] = { -13.0f, -13.445f, 3.001f };
    float actual_vel[] = { 0.0f, 0.0f, 0.0f };
    float default_vel[] = { 0.0f, 0.0f, 0.0f };
    float orien[] = { 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f };
    float actual_orien[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    float default_orien[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
    ASSERT_OK(seal_startup(NULL));

    seal_get_listener_pos(actual_pos, actual_pos + 1, actual_pos + 2);
    ASSERT(memcmp(default_pos, actual_pos, sizeof (actual_pos)) == 0);
    seal_set_listener_pos(pos[0], pos[1], pos[2]);
    seal_get_listener_pos(actual_pos, actual_pos + 1, actual_pos + 2);
    ASSERT(memcmp(pos, actual_pos, sizeof (actual_pos)) == 0);

    seal_get_listener_vel(actual_vel, actual_vel + 1, actual_vel + 2);
    ASSERT(memcmp(default_vel, actual_vel, sizeof (vel)) == 0);    
    seal_set_listener_vel(vel[0], vel[1], vel[2]);
    seal_get_listener_vel(actual_vel, actual_vel + 1, actual_vel + 2);
    ASSERT(memcmp(vel, actual_vel, sizeof (actual_vel)) == 0);

    seal_get_listener_orien(actual_orien);
    ASSERT(memcmp(default_orien, actual_orien, sizeof (actual_orien)) == 0);
    seal_set_listener_orien(orien);
    seal_get_listener_orien(actual_orien);
    ASSERT(memcmp(orien, actual_orien, sizeof (actual_orien)) == 0);

    ASSERT_OK(seal_get_listener_gain() == 1.0f);
    ASSERT_OK(seal_set_listener_gain(332.0321f));
    ASSERT_FAIL(!seal_set_listener_gain(-0.15f), SEAL_BAD_LISTENER_ATTR_VAL);
    ASSERT_OK(seal_get_listener_gain() == 332.0321f);

    seal_cleanup();
}

void
test_listener(void)
{
    test_listener_attr();
}