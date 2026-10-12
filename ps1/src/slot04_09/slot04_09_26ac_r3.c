/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_grab_state_save_other_image_slot04_09(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p;
    o->other_lock = 0x10;
    p = o->other;
    o->state_3++;
    o->field_157 = 0;
    obj->field_334 = p->field_0c;
    obj->field_335 = p->image_row;
    spawn_offset_effect_and_play_sounds(o);
    set_sequence_by_side_and_mode(o, 0x28);
}

void branch_on_step_flag_bit_one_slot04_09(Object *obj) {
    update_sequence(obj);
    if (obj->step_flags & 1) {
        if (obj->field_241 != 0) {
            obj->state_3 = 6;
            obj->other->field_6b = 0;
            set_sequence_by_side_and_mode(obj, 0x2a);
        } else {
            obj->countdown = 8;
            obj->vel_x = 0x98000;
            obj->vel_y = 0;
            obj->accel_x = 0;
            obj->accel_y = 0;
            obj->state_3++;
            obj->step_flags = obj->step_flags & 0xff00;
            if (obj->facing_side == 0) {
                obj->vel_x = 0xfff68000;
            }
        }
    }
}
