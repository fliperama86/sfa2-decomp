/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6238_slot04_02(Object *object);

void func_801b57b0_slot04_02(Object *obj) {
    Object *p;
    int a;
    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_45 != 0) {
        a = 0x2f;
    } else {
        p = (Object *)func_8011f1e0();
        a = 0x2b;
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x7d;
            p->field_3c = obj;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_0e = obj->field_0e;
            obj->field_3c = p;
            p->field_02 = 9;
            p->field_08 = 0x20;
            p->field_66 = obj->field_66;
        }
    }
    func_80130678(obj, a);
}

void func_801b5884_slot04_02(Object *obj) {
    int a;
    int b;
    if ((s16)obj->field_3a != 0) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 != 0) {
            a = 0xd;
            b = 0x39;
        } else {
            a = 1;
            b = 0x47;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b6238_slot04_02(obj);
    }
    func_80130efc(obj);
}
