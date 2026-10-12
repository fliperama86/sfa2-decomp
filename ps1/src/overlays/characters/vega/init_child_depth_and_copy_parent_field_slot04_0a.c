/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void noop_slot00_2_slot04_0a(Object *obj);
void noop_slot00_3_slot04_0a(Object *obj);

void init_child_depth_and_copy_parent_field_slot04_0a(Object *obj) {
    obj->ot_depth = 4;
    obj->spawn_mode = 5;
    obj->field_0c = 0xff;
    obj->state_0++;
    obj->field_1c = obj->parent->field_1c;
}

void update_countdown_check_parent_state_slot04_0a(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->countdown = 0;
        if (obj->vel_y != ((obj->parent->state_0 << 24) | (obj->parent->state_1 << 16) | (obj->parent->state_2 << 8))) {
            obj->state_0 = 2;
        } else {
            if (obj->step_flags & 0x100) {
                noop_slot00_2_slot04_0a(obj);
            } else if (obj->step_flags & 0x200) {
                noop_slot00_3_slot04_0a(obj);
            }
            set_flag_and_push_if_mode_8(obj);
        }
    }
}
