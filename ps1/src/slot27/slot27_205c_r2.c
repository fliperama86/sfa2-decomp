/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_palette_animation_and_upload_slot27(Object *obj);

void wait_y_scale_flag_then_advance_state_1_slot27(Object *obj) {
    if (!(obj->y_scale & 0x8000)) {
        advance_palette_animation_and_upload_slot27(obj);
    } else {
        obj->state_2 = 0;
        obj->state_1 = obj->state_1 + 1;
        obj->parent->active_count = 2;
    }
}
