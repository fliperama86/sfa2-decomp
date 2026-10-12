/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801dd5b0_slot05_06[])(Object *);
extern Object *data_801dd754_slot05_06;
void func_801cdc58_slot05_06(Object *obj);

void func_801cdac4_slot05_06(Object *obj) {
    u16 t;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    func_80141f28(obj, obj->direction_index >> 1);
    t = 0x12;
    if (obj->variant_index == 0) {
        t = 0xc;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    t = (obj->direction_index >> 1) + t;
    func_801307e0(obj, t);
}

void func_801cdb44_slot05_06(Object *obj) {
    func_8011f14c((Slab172 *)obj);
}

void func_801cdb64_slot05_06(Object *obj) {
    data_801dd754_slot05_06 = obj->parent;
    data_801dd5b0_slot05_06[obj->state_0](obj);
}

void func_801cdbb4_slot05_06(Object *obj) {
    Object *p;

    obj->state_0++;
    p = data_801dd754_slot05_06;
    obj->field_1c = p->field_1c;
    obj->cycle_value = p->cycle_value;
    obj->table_index = p->kind;
    obj->field_0c = data_801dd754_slot05_06->field_0c;
    obj->image_row = data_801dd754_slot05_06->image_row;
    obj->field_0e = data_801dd754_slot05_06->field_0e;
    obj->variant_index = 0;
    func_801cdc58_slot05_06(obj);
}
