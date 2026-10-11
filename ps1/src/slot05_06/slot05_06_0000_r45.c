/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd498_slot05_06[];
void func_801cc61c_slot05_06(Object *object);
extern ObjectFn data_801dd4a0_slot05_06[];

void func_801cbcf4_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int v;

    obj->field_1c6 -= 1;
    if (obj->field_1c6 & 0x80) {
        o->state_3++;
        func_801307e0(o, 0x33);
    } else {
        v = 0x80000;
        if (o->field_0b == 0) {
            v = -0x80000;
        }
        *(s32 *)&o->pos_x_frac = v + *(s32 *)&o->pos_x_frac;
        update_sequence(o);
    }
}

void func_801cbd6c_slot05_06(Object *obj) {
    data_801dd498_slot05_06[obj->state_3](obj);
}

void func_801cbdac_slot05_06(Object *obj) {
    obj->field_17b = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_157 = 0;
    func_801307e0(obj, 0x3c);
}

void func_801cbdfc_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    if (obj->field_3b & 0x80) {
        func_801cc61c_slot05_06(o);
    } else {
        update_sequence(o);
    }
}

void func_801cbe40_slot05_06(Object *obj) {
    data_801dd4a0_slot05_06[obj->state_3](obj);
}
