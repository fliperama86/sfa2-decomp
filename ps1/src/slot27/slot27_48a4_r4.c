/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot27Rec7d14 data_80027d14_slot27[];
extern SequenceStep *data_80027fa8_slot27[];
extern ObjectFn data_80028110_slot27[];
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];

void init_object_from_table_index_entry_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    o->field_90 = (void *)0x80038000;
    o->data_ptr_a = data_80017c28_slot27;
    o->data_ptr_b = data_8001aa14_slot27;
    o->field_01 = 0;
    o->spawn_x = 0;
    o->spawn_y = 0x1e0;
    o->image_row = 0;
    o->facing_side = 0;
    o->step_flags = 0;
    o->ot_depth = data_80027d14_slot27[o->table_index].ot_depth;
    obj->angle = data_80027d14_slot27[o->table_index].field_02;
    *(u16 *)&o->pos_y = data_80027d14_slot27[o->table_index].init_pos_y;
}

void set_sequence_from_table_index_slot27(Object *obj) {
    set_sequence_from_table_5(obj, obj->table_index, data_80027fa8_slot27);
}

void call_handler_by_state_0_with_refs_slot27(Object *obj) {
    ref_other.p = obj->parent;
    (*(Object **)&data_80190458) = obj->other;
    data_80028110_slot27[obj->state_0](obj);
}
