/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6e18_slot04_02(Object *obj);
void func_801b6e68_slot04_02(Object *obj);
void func_801b6ec0_slot04_02(Object *obj);

void func_801b6d08_slot04_02(Object *o) {
    Object *p = o->field_3c;
    o->field_04++;
    o->field_1c = p->field_1c;
    o->field_03 = p->kind;
    o->field_0c = p->field_0c;
    o->field_0d = p->field_0d;
    o->field_0e = p->field_0e;
    o->field_7a = 0x60;
    o->field_48 = 0;
    o->field_7c = 0x1e0;
    o->field_09 = 0;
}

void func_801b6d6c_slot04_02(Object *o) {
    Object *p = o->field_3c;
    int s;
    o->field_01 = 0;
    if (o->field_03 != p->kind) {
        func_801b6e18_slot04_02(o);
    } else {
        s = p->frame->field_09;
        if (s == 0) {
            o->field_48 = 0;
        } else {
            func_801b6ec0_slot04_02(o);
            o->field_01 = 1;
            if (o->field_48 != s) {
                o->field_48 = s;
                func_801b6e68_slot04_02(o);
            } else {
                func_80131094(o);
            }
        }
    }
}
