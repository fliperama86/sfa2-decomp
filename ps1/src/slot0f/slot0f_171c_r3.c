/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_800e0850_slot0f(int a, s16 *p);
void func_800e2550_slot0f(Object *o);
void func_800e26cc_slot0f(Object *o);

void func_800e1964_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u16 b;
    func_800e2550_slot0f(o);
    b = o->field_10;
    if (b & 0x860) {
        if (b & 0x820) {
            if (obj->field_0c == 0) {
                obj->field_0f = 2;
                o->field_01 += 1;
                o->field_04 += 1;
            } else {
                o->field_01 = 1;
                o->field_04 = 0;
            }
            func_80120554(0, 0, 0x205);
        } else {
            o->field_01 = 1;
            o->field_04 = 0;
            func_80120554(0, 0, 0x202);
        }
    }
}

void func_800e1a10_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    s8 t;
    if (obj->field_0f != 0) {
        obj->field_0f = obj->field_0f - 1;
    } else {
        func_800e26cc_slot0f(o);
        if (obj->field_0b == 0) {
            o->field_0e = func_800e0850_slot0f(0, &o->pos_x);
        } else {
            o->field_0e = func_800e0850_slot0f(0x10, &o->pos_x);
        }
        t = -0x4c;
        o->field_0f = t;
        o->field_01 += 1;
        o->field_04 += 1;
    }
}
