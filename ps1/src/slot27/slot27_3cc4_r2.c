/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80026ed0_slot27[])(Object *);
extern void (*data_80026ee0_slot27[])(Object *);
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern void func_8001435c_slot27(Object *obj);
extern void func_80014298_slot27(Object *obj);
extern SequenceStep *data_80027bc4_slot27[];

void func_80013ee4_slot27(Object *object) {
    data_80026ed0_slot27[object->state_0](object);
}

void func_80013f24_slot27(Object *object) {
    data_80026ee0_slot27[object->state_1](object);
}

void func_80013f64_slot27(Object *obj) {
    Object *o;

    obj->ot_depth = 8;
    obj->data_ptr_a = data_80017c28_slot27;
    obj->data_ptr_b = data_8001aa14_slot27;
    obj->spawn_y = 0x1e0;
    obj->countdown = 0x40ff;
    obj->field_90 = (void *)0x80038000;
    obj->spawn_x = 0;
    obj->image_row = 0;
    obj->field_01 = 0;
    obj->state_1 = obj->state_1 + 1;
    o = obj->other;
    obj->image_row = o->side;
    obj->facing_side = obj->image_row ^ 1;
    obj->table_index = o->kind;
    obj->variant_index = obj->table_index;
    func_8001435c_slot27(obj);
    func_80014298_slot27(obj);
}
