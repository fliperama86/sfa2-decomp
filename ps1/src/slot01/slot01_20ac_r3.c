/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002ceb8_slot01[];
void func_80012438_slot01(Object *obj);
void func_800124f4_slot01(Object *obj);
void func_80012714_slot01(Object *obj, u8 *a, int b, int c);

void func_80012344_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    Object *s = o->field_3c;
    if (game_state.field_ab & 0x80) {
        func_80012438_slot01(o);
    } else {
        int t;
        obj->field_10 += o->field_4c;
        o->field_4c += o->field_54;
        obj->field_14 += o->field_50;
        o->field_50 += o->field_58;
        o->field_24 += 8;
        if (o->field_46 & 1) {
            o->field_20++;
            o->field_22++;
        }
        t = o->field_46 - 1;
        o->field_46 = t;
        if ((s16)t < 0) {
            func_80012438_slot01(o);
        }
    }
    func_80012714_slot01(o, data_8002ceb8_slot01, s->side, data_801a27d0);
}

void func_80012438_slot01(Object *obj) {
    obj->field_24 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_04++;
    obj->pos_x = obj->field_5c;
    obj->pos_y = obj->field_5e;
    func_800124f4_slot01(obj);
}

void func_8001247c_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    Object *child = obj->field_30;
    o->field_04 = 3;
    if (child != 0) {
        child->field_04 = 2;
    }
}

void func_8001249c_slot01(Object *obj) {
    func_80012714_slot01(obj, data_8002ceb8_slot01, obj->field_3c->side, data_801a27d0);
}
