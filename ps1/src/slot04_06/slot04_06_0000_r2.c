/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int index);
Block172 *func_8011f1e0(void);

void func_801b00f8_slot04_06(Object *obj) {
    Object *c;
    u8 d;

    if (*((u8 *)&obj->field_3a + 1) & 0x80) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
        func_80130678(obj, 0);
    } else {
        if (*(u8 *)&obj->field_3a != 0) {
            *(u8 *)&obj->field_3a = 0;
            c = (Object *)func_8011f1e0();
            if (c != 0) {
                c->field_00 = 1;
                c->field_03 = 4;
                c->field_3c = obj;
                c->field_1c = obj->field_1c;
                c->field_7a = obj->field_7a;
                c->field_7c = obj->field_7c;
                d = obj->field_0d;
                c->field_02 = 0x11;
                c->field_08 = 0x20;
                c->field_0d = d;
                c->field_90 = obj->field_90;
                c->field_66 = obj->field_66;
                c->field_98 = obj->field_98;
                c->field_9c = obj->field_9c;
            }
        }
        func_80130efc(obj);
    }
}
