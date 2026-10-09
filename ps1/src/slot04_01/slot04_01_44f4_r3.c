/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

void func_801b47cc_slot04_01(Object *obj, int kind) {
    Object *p;
    int a = -0x18;
    u8 t;

    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 1;
        p->field_03 = game_state.field_1d;
        p->field_0e = obj->field_0e;
        p->field_09 = 4;
        p->field_48 = kind;
        p->field_3c = obj;
        p->field_0c = obj->field_0c;
        p->field_1c = obj->field_1c;
        p->field_7a = obj->field_7a;
        p->field_7c = obj->field_7c;
        p->field_0d = obj->field_0d;
        p->field_08 = 0x20;
        p->field_66 = obj->field_66;
        p->field_90 = obj->field_90;
        p->field_98 = obj->field_98;
        p->field_9c = obj->field_9c;
        if (kind != 7) {
            a = 0x14;
        }
        t = obj->field_0b;
        p->field_0b = t;
        if (t != 0) {
            p->pos_x = a + p->pos_x;
        } else {
            p->pos_x = p->pos_x - a;
        }
        p->pos_y = obj->pos_y - 0x74;
    }
}
