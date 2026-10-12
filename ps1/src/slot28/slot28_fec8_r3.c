/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void apply_velocity_and_accel_slot28(Object *o);

void move_until_x_80_then_next_state_1_slot28(Object *obj) {
    apply_velocity_and_accel_slot28(obj);
    if (obj->pos_x >= 0x80) {
        obj->state_3 = 1;
        obj->pos_x = 0x80;
        obj->state_1++;
    }
}

void func_8002045c_slot28(Object *obj) {
    apply_velocity_and_accel_slot28(obj);
    if (obj->pos_x >= 0x89) {
        obj->pos_x = 0x88;
        obj->vel_x = 0x4000;
        obj->vel_y = -0x4000;
        obj->accel_x = 0;
        obj->accel_y = -0x400;
        obj->state_1++;
        set_sequence_from_table_5(obj, 1, (SequenceStep **)obj->box_tables);
    }
}

void move_until_y_140_then_next_state_0_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;

    if (*(u8 *)&obj->field_3a == 0) {
        apply_velocity_and_accel_slot28(o);
        if (o->pos_y >= 0x140) {
            o->state_0++;
        }
    }
}
