/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b7a4c_slot04_02(Object *obj);
void func_801b7b10_slot04_02(Object *obj);
void func_801b7b44_slot04_02(Object *obj);
void func_801b7b74_slot04_02(Object *obj);

void func_801b79b4_slot04_02(Object *obj) {
    Object *p = obj->field_3c;

    obj->field_04++;
    obj->field_1c = p->field_1c;
    obj->field_03 = p->kind;
    obj->field_0c = p->field_0c;
    obj->field_0d = p->field_0d;
    obj->field_0e = p->field_0e;
    obj->field_7a = 0x60;
    obj->field_48 = 0;
    obj->field_7c = 0x1e0;
    obj->field_90 = p->field_90;
    obj->field_98 = p->field_98;
    obj->field_9c = p->field_9c;
    func_801b7a4c_slot04_02(obj);
}

void func_801b7a4c_slot04_02(Object *obj) {
    Object *p = obj->field_3c;
    u16 t;
    u16 u;
    u16 w;

    obj->field_01 = 0;
    if (obj->field_03 != p->kind) {
        func_801b7b10_slot04_02(obj);
    } else {
        w = p->field_3a;
        t = w & 0xff;
        u = t;
        if (u == 0) {
            obj->field_48 = 0;
        } else if ((w & 0x80) != 0) {
            func_801b7b10_slot04_02(obj);
        } else {
            func_801b7b74_slot04_02(obj);
            obj->field_01 = 1;
            if (obj->field_48 != u) {
                obj->field_48 = t;
                func_801b7b44_slot04_02(obj);
            } else {
                func_80131094(obj);
                func_8011ffdc(obj);
            }
        }
    }
}
