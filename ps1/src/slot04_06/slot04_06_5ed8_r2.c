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

    obj->field_04++;
    obj->field_0c = data_801c575c_slot04_06->field_0c;
    obj->field_0d = data_801c575c_slot04_06->field_0d;
    p = data_801c575c_slot04_06;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->field_0e = p->field_0e;
    obj->field_09 = 6;
    obj->field_0b = data_801c575c_slot04_06->field_0b;
    ((Slot04aObj *)obj)->field_70 = ((Slot04aObj *)data_801c575c_slot04_06)->field_70;
    func_801b6264_slot04_06(obj);
    data_801c55e4_slot04_06[obj->field_03](obj);
    func_801b6218_slot04_06(obj);
}

void func_801b619c_slot04_06(Object *obj) {
    obj->field_09 = 1;
    obj->field_48 = 0;
}

void func_801b61ac_slot04_06(Object *obj) {
    obj->field_09 = 1;
    obj->field_48 = 0x10;
}
