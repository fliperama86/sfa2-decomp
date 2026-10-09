/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void func_801b01ec_slot04_07(Object *obj) {
    u16 t = obj->field_3a;
    int i;
    Object *c;

    if ((t & 0xff) != 0) {
        obj->field_3a = t & 0xff00;
        for (i = 0; i < 3; i++) {
            c = (Object *)func_8011f1e0();
            if (c != 0) {
                c->field_00 = 1;
                c->field_02 = 7;
                c->other = obj;
                c->field_03 = i;
                c->field_7a = obj->field_7a;
                c->field_7c = obj->field_7c;
                c->field_0d = obj->field_0d;
                c->field_08 = 0x20;
                c->field_66 = obj->field_66;
                c->field_90 = obj->field_90;
                c->field_98 = obj->field_98;
                c->field_9c = obj->field_9c;
            }
        }
    }
    func_801b41c4_slot04_07(obj);
    if (obj->field_50 >= 0 || (i = obj->field_70, obj->pos_y < i)) {
        func_80130efc(obj);
    } else {
        obj->field_06 += 1;
        obj->pos_y = i;
        if (obj->side == 0) {
            obj->pos_x = 0x220;
        } else {
            obj->pos_x = 0x2e0;
        }
        obj->field_4c = 0;
        obj->field_50 = 0;
        obj->field_54 = 0;
        obj->field_58 = 0;
        obj->field_10 = 0;
        obj->field_14 = 0;
    }
}
