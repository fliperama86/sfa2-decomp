/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1b04_slot04_09(Object *obj) {
    int n = 0x1c;

    obj->field_17b = 1;
    obj->field_225 = 1;
    obj->field_07++;
    if (obj->field_246 == 0) {
        obj->field_225 = 0;
        func_80141f28(obj, 4);
    }
    func_80138ae8(&game_state, obj);
    func_801204f4(obj, obj->side, 6);
    func_801204f4(obj, obj->side, 0x16);
    if (obj->field_49 != 0) {
        n = 0x40;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + n);
}

void func_801b1bb8_slot04_09(Object *obj) {
    Object *p;

    func_80130efc(obj);
    if (*(s16 *)&obj->field_3a != 0) {
        p = func_8011f0e8(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x16;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_ac = obj->field_12a;
            p->field_ad = 0;
            p->field_0e = obj->field_0e;
            p->field_26 = obj->field_26;
            *(s32 *)&p->field_a4 = 0x1000700;
            p->field_7a = 0x60;
            p->field_3c = obj;
            p->field_7c = 0x1e0;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
        }
        obj->field_07++;
    }
}
