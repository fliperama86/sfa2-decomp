/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void queue_five_jobs(void) {
    queue_image_upload_job(0, 6);
    queue_image_upload_job(1, 0);
    queue_image_upload_job(2, 1);
    queue_image_upload_job(3, 2);
    queue_image_upload_job(4, 7);
}

void update_object_motion_unless_paused(Object *object) {
    if ((game_state.config->pause_flag_a | game_state.config->pause_flag_b) != 0) {
        func_8011ffdc(object);
    } else {
        *(s32 *)&object->pos_x_frac += object->vel_x;
        *(u32 *)&object->pos_y_frac -= object->vel_y;
        advance_sequence_checked(object);
        set_state_0_if_x_outside_margin_narrow(object);
        func_8011ffdc(object);
    }
}

void call_state_2_handler_267c_unless_paused(Object *object) {
    if ((game_state.config->pause_flag_a | game_state.config->pause_flag_b) != 0) {
        func_8011ffdc(object);
    } else {
        handlers_267c[object->state_2](object);
        func_8011ffdc(object);
    }
}

void func_80137c54(Object *object) {
    object->state_2++;
    object->countdown = (object->countdown & 0xff) | 0x200;
    advance_sequence_checked(object);
}
