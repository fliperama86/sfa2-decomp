/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801e1fdc_slot0b[])(Object *);
extern u8 data_801e766c_slot0b[];
void func_801e01dc_slot0b(Object *obj);
SlotCell *func_801e101c_slot0b(Object *obj, SlotCell *cell, int arg, int base);

void func_801e00c4_slot0b(Object *obj) {
    ref_other.p = obj->parent;
    data_801e1fdc_slot0b[obj->state_0](obj);
}

void func_801e0114_slot0b(Object *obj) {
    int s;
    int r;

    obj->facing_side = 0;
    obj->field_0e = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->table_index_2 = 0;
    obj->state_0 = obj->state_0 + 1;
    if (obj->table_index == 0) {
        obj->field_01 = 0;
        obj->field_0c = 0;
        obj->ot_depth = 3;
        func_80130768(obj, 0, (SequenceStep **)((ObjectWords *)obj)->word_58);
        s = func_8015bd0c(0, 0, 0x280, 0) & 0xffff;
        r = func_8015bdd4(0x140, 0x1e6);
        func_801e101c_slot0b(obj, (SlotCell *)(data_801e766c_slot0b + obj->variant_index * 0x1e0), s, r & 0xffff);
        func_801e01dc_slot0b(obj);
    }
}
