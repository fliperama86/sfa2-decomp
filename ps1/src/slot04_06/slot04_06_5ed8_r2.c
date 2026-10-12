/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c575c_slot04_06;
extern ObjectFn data_801c55e4_slot04_06[];
void func_801b6264_slot04_06(Object *obj);
void func_801b6218_slot04_06(Object *obj);

void func_801b60a4_slot04_06(Object *obj) {
    Object *p;

    obj->state_0++;
    obj->field_0c = data_801c575c_slot04_06->field_0c;
    obj->image_row = data_801c575c_slot04_06->image_row;
    p = data_801c575c_slot04_06;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->field_0e = p->field_0e;
    obj->ot_depth = 6;
    obj->facing_side = data_801c575c_slot04_06->facing_side;
    ((Slot04aObj *)obj)->saved_pos_y = ((Slot04aObj *)data_801c575c_slot04_06)->saved_pos_y;
    func_801b6264_slot04_06(obj);
    data_801c55e4_slot04_06[obj->table_index](obj);
    func_801b6218_slot04_06(obj);
}

void func_801b619c_slot04_06(Object *obj) {
    obj->ot_depth = 1;
    obj->variant_index = 0;
}

void func_801b61ac_slot04_06(Object *obj) {
    obj->ot_depth = 1;
    obj->variant_index = 0x10;
}
