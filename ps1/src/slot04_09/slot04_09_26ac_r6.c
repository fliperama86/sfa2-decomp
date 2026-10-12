/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void apply_velocity_and_acceleration_slot04_09(Object *o);
void func_801b2d6c_slot04_09(Object *o);

void fall_until_ground_then_random_sequence_slot04_09(Object *obj) {
    func_801b2d6c_slot04_09(obj);
    apply_velocity_and_acceleration_slot04_09(obj);
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y_frac = 0;
        obj->pos_y = obj->ground_y;
        obj->other->active_state = 0;
        if (get_random_byte() & 1) {
            set_sequence_by_side_and_mode(obj, 0x35);
        } else {
            set_sequence_by_side_and_mode(obj, 0x36);
        }
    }
}

void func_801b2c90_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        Object *p = obj->other;
        p->field_0c = ((Slot04aObj *)obj)->field_334;
        p->image_row = ((Slot04aObj *)obj)->field_335;
        obj->active_flag_3 = 0;
        obj->state_3 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        func_801b2d6c_slot04_09(obj);
        update_sequence(obj);
    }
}

void end_sequence_restore_other_image_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        Object *p = obj->other;
        p->field_0c = ((Slot04aObj *)obj)->field_334;
        p->image_row = ((Slot04aObj *)obj)->field_335;
        obj->active_flag_3 = 0;
        obj->state_3 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
