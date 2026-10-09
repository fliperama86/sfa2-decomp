/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Block172 *func_8011f1e0(void);

void func_80146478(Object *object, u8 a, int dx, int dy) {
    Object *o;
    s16 x;

    x = dx;
    o = (Object *)func_8011f1e0();
    if (o != 0 && data_801a27d4 != 0) {
        o->field_00 = 1;
        o->field_02 = 4;
        o->field_0e = object->field_0e;
        o->field_48 = 0;
        o->field_65 = object->field_65;
        o->field_3c = object->other;
        o->pos_x = object->pos_x;
        o->pos_y = object->pos_y;
        o->field_0b = object->field_0b;
        if (o->field_0b != 0) {
            x = -dx;
        }
        o->field_03 = a;
        o->field_01 = 0;
        o->field_76 = 0x240;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_90 = (void *)0x800fb100;
        o->field_78 = 0;
        o->pos_x = o->pos_x + x;
        o->pos_y = o->pos_y - dy;
        data_801a27d4--;
    }
}

void func_801465b0(Object *object, int a, int dx, int dy) {
    Object *o;
    Object *other;

    o = (Object *)func_8011f1e0();
    if (o != 0 && data_801a27d4 != 0) {
        other = object->other;
        o->field_00 = 1;
        o->field_02 = 4;
        o->field_0e = other->field_0e;
        o->field_48 = 0;
        o->field_65 = object->field_65;
        o->field_3c = other;
        o->pos_x = other->pos_x;
        o->pos_y = other->pos_y;
        o->field_0b = other->field_0b;
        if (o->field_0b != 0) {
            dx = -dx;
        }
        o->field_03 = a;
        o->field_01 = 0;
        o->field_76 = 0x240;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_98 = &data_80172a48;
        o->field_9c = &data_80173c9c;
        o->field_90 = (void *)0x800fb100;
        o->field_78 = 0;
        o->pos_x = o->pos_x + dx;
        o->pos_y = o->pos_y - dy;
        data_801a27d4--;
    }
}
