/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801e4710_slot0b[])(ModObj *);
extern void (*data_801e471c_slot0b[])(ModObj *);
extern u8 *data_801e46b4_slot0b[];
extern ObjectRef data_80190468;
void func_801e06b0_slot0b(ModObj *obj);
void func_801e0a78_slot0b(ModObj *obj, void *a, int b, int c);

void func_801e05a0_slot0b(Object *o) {
    ModObj *obj = (ModObj *)o;
    data_801e4710_slot0b[obj->field_03](obj);
}

void func_801e05e0_slot0b(ModObj *obj) {
}

void func_801e05e8_slot0b(ModObj *obj) {
    ref_other.p = obj->field_3c;
    if (data_80190468.p->field_04 == 0) {
        data_801e471c_slot0b[obj->field_05](obj);
        if (data_80190a40 != 1) {
            func_801e0a78_slot0b(obj, data_801e46b4_slot0b[ref_other.p->side], ref_other.p->side, data_801a27d0);
        }
    } else {
        func_801e06b0_slot0b(obj);
    }
}

void func_801e06b0_slot0b(ModObj *obj) {
    ModObj *child = obj->field_30;
    obj->field_04 = 3;
    if (child != 0) {
        child->field_04 = 2;
    }
}
