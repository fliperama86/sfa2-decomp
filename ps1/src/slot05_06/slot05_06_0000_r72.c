/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd6a0_slot05_06[];
extern ObjectFn data_801dd6c0_slot05_06[];
int func_801ce6f8_slot05_06(Object *obj);

void func_801ce330_slot05_06(Object *obj) {
    if (game_state.field_6a != 0) {
        obj->field_04 = 2;
    } else {
        data_801dd6a0_slot05_06[obj->field_03](obj);
    }
}

void func_801ce388_slot05_06(Object *obj) {
    int t;
    int a;

    t = obj->field_48 + 1;
    obj->field_48 = t & 0x1f;
    a = 1;
    if (t & 0x10) {
        a = -1;
    }
    obj->pos_y -= a;
    func_80131094(obj);
    func_8011ffdc(obj);
}

void func_801ce3e8_slot05_06(Object *o) {
    data_801dd6c0_slot05_06[o->field_05](o);
    func_8011ffdc(o);
}

void func_801ce43c_slot05_06(Object *obj) {
    if ((func_801ce6f8_slot05_06(obj) << 16) > 0) {
        obj->field_4c = 0x2000;
        if ((obj->field_0b ^ (obj->field_03 & 1)) != 0) {
            obj->field_4c = -0x2000;
        }
        obj->field_50 = 0x1c000;
        obj->field_58 = -0x1800;
        obj->pos_y = ((Slot04aObj *)obj)->field_70;
        obj->field_05++;
    }
    func_80131094(obj);
}
