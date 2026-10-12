/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801dd5d0_slot05_06[])(Object *);
extern Object *data_801dd758_slot05_06;
extern ObjectFn data_801dd5e0_slot05_06[];
void func_801ce260_slot05_06(Object *obj);
void func_801ce214_slot05_06(Object *obj);
void func_801ce2c0_slot05_06(Object *obj);

void func_801ce050_slot05_06(Object *obj) {
    data_801dd758_slot05_06 = obj->parent;
    data_801dd5d0_slot05_06[obj->state_0](obj);
}

void func_801ce0a0_slot05_06(Object *obj) {
    Object *p;

    obj->state_0++;
    obj->field_0c = data_801dd758_slot05_06->field_0c;
    obj->image_row = data_801dd758_slot05_06->image_row;
    p = data_801dd758_slot05_06;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->field_0e = p->field_0e;
    obj->ot_depth = 6;
    obj->facing_side = data_801dd758_slot05_06->facing_side;
    ((Slot04aObj *)obj)->saved_pos_y = ((Slot04aObj *)data_801dd758_slot05_06)->saved_pos_y;
    func_801ce260_slot05_06(obj);
    data_801dd5e0_slot05_06[obj->table_index](obj);
    func_801ce214_slot05_06(obj);
}

void func_801ce198_slot05_06(Object *obj) {
    obj->ot_depth = 1;
    obj->variant_index = 0;
}

void func_801ce1a8_slot05_06(Object *obj) {
    obj->ot_depth = 1;
    obj->variant_index = 0x10;
}

void func_801ce1bc_slot05_06(Object *obj) {
    func_801ce2c0_slot05_06(obj);
}
