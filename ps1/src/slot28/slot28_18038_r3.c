/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HalfPair data_800517fc_slot28[];

void init_object_rising_from_position_table_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    obj->field_01 = 1;
    obj->vel_y = 0x10000;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0 = obj->state_0 + 1;
    obj->pos_x = data_800517fc_slot28[obj->table_index].pos_x;
    obj->pos_y = data_800517fc_slot28[obj->table_index].pos_y;
    set_sequence_from_table_5(obj, 0, (SequenceStep **)obj->box_tables);
}

void add_vel_y_clamp_then_advance_sequence_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    int sum = obj->pos_y + o->vel_y;

    obj->pos_y = sum;
    if (sum > 0xffffff) {
        obj->pos_y = 0xff800000;
    }
    advance_sequence_checked(o);
}
