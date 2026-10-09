/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9a84_slot06_01(Object *obj, u8 index);

void func_801e9988_slot06_01(Object *obj) {
    int index = 6;
    u8 t;

    obj->field_0f = 1;
    obj->field_46 = 0x3f;
    obj->field_76 = 0x300;
    obj->field_78 = 0x100;
    obj->field_7a = 0x60;
    obj->field_7c = 0x1e0;
    t = obj->field_04;
    obj->field_58 = 0;
    obj->field_0d = 0;
    obj->field_81 = 4;
    obj->field_04 = t + 1;
    *(u16 *)&obj->pos_y = 0xf8 - (u16)obj->pos_y;
    if (obj->field_03 != 0) {
        Object *p = &player_left;

        if (p->field_cd != 0 || p->kind != 1) {
            p++;
            if (p->field_cd != 0 || p->kind != 1) {
                func_801e9a84_slot06_01(obj, 0);
                return;
            }
        }
        index = 0x11;
        obj->field_3c = p;
        obj->field_58 = 1;
    }
    func_801e9a84_slot06_01(obj, index);
}
