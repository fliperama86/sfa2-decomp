/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_palette_animation_and_upload_slot27(Object *obj);

void move_pos_x_until_target_and_step_palette_slot27(Object *obj) {
    s32 *t = (s32 *)data_8019045c;
    *t = obj->vel_x;
    obj->pos_x = obj->pos_x + *(u16 *)t;
    *t = obj->pos_x;
    if (obj->vel_y == *t) {
        obj->state_2 = obj->state_2 + 1;
    }
    advance_palette_animation_and_upload_slot27(obj);
}
