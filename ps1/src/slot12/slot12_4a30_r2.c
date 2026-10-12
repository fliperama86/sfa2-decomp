/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80014dd0_slot12(Object *obj, int a);
u8 move_with_accel_and_check_ground_slot12(Object *obj);

void advance_state_1_on_ground_landing_slot12(Object *obj) {
    if (move_with_accel_and_check_ground_slot12(obj) & 0xff) {
        obj->state_1++;
        obj->pos_y = obj->ground_y;
        func_80014dd0_slot12(obj, 0);
    } else {
        advance_sequence_checked(obj);
    }
}

u8 move_with_accel_and_check_ground_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    *(s32 *)&o->pos_x_frac += obj->field_4c;
    obj->field_4c += obj->field_54;
    *(s32 *)&o->pos_y_frac += obj->field_50;
    obj->field_50 += obj->accel_y_step;
    return o->pos_y >= o->ground_y;
}

void advance_state_1_on_sequence_end_slot12(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->countdown = 10;
        obj->state_1++;
        func_80014dd0_slot12(obj, 0);
    } else {
        advance_sequence_checked(obj);
    }
}
