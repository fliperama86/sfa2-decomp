/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Block172 *func_8011f1e0(void);

int func_80146ba0(Object *object, u8 index) {
    s8 tbl[7] = {0x15, 0x16, 0x30, 0x0c, 0x0d, 0x0a, 0x12};
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
    ref_other.p->field_00 = 1;
    ref_other.p->field_02 = 5;
    ref_other.p->field_03 = 0;
    ref_other.p->field_0e = object->field_0e;
    ref_other.p->field_09 = 4;
    ref_other.p->field_48 = tbl[index];
    ref_other.p->pos_x = object->pos_x;
    ref_other.p->pos_y = object->pos_y;
    ref_other.p->field_0b = object->field_0b;
    ref_other.p->field_1c = object->field_1c;
    ref_other.p->field_0c = 0;
    return 1;
    }
    return 0;
}

int func_80146cf8(Object *object, u8 index) {
    s8 tbl[2] = {0x10, 0x11};
    Object *other;
    other = object->other;
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
    ref_other.p->field_00 = 1;
    ref_other.p->field_02 = 5;
    ref_other.p->field_03 = tbl[index];
    ref_other.p->field_0e = other->field_0e;
    ref_other.p->field_09 = 0;
    ref_other.p->field_48 = 0x16;
    ref_other.p->pos_x = other->pos_x;
    ref_other.p->pos_y = other->field_70;
    ref_other.p->field_0b = 0;
    ref_other.p->field_1c = object->field_1c;
    ref_other.p->field_0c = 0;
    return 1;
    }
    return 0;
}

int func_80146e3c(Object *object, u8 index) {
    s8 tbl[10] = {0x10, 0x11, 0x0f, 0x0b, 0x0c, 0x0d, 0x0e, 0x20, 0x21, 0x22};
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
    ref_other.p->field_00 = 1;
    ref_other.p->field_02 = 5;
    ref_other.p->field_03 = 0;
    ref_other.p->field_0e = object->field_0e;
    ref_other.p->field_09 = 0;
    ref_other.p->field_48 = tbl[index];
    ref_other.p->field_0b = object->field_0b;
    ref_other.p->pos_x = object->pos_x;
    ref_other.p->pos_y = object->pos_y;
    ref_other.p->field_1c = object->field_1c;
    ref_other.p->field_0c = 0;
    ref_other.p->pos_x = ref_other.p->pos_x + table_8017cac4[func_80151184() & 0xf];
    ref_other.p->pos_y = ref_other.p->pos_y - table_8017cac4[func_80151184() & 0xf];
    return 1;
    }
    return 0;
}

int func_80147000(Object *object) {
    Object *o;
    Object *src;
    u8 w;
    u16 v;

    if (game_state.field_ae == 0) {
        game_state.field_ae = 0xff;
        o = (Object *)func_8011f1e0();
        if (o == 0) {
            return 0;
        }
        src = object->field_08 == 8 ? object->field_3c : object;
        o->field_00 = 1;
        if (game_state.field_6b == 6) {
            func_8014780c(object);
        }
        o->field_02 = 6;
        o->field_03 = game_state.field_6b;
        o->field_3c = object;
        o->field_67 = src->field_0b;
        o->field_1c = src->field_1c;
        w = src->field_255;
        o->field_98 = &data_80172a48;
        o->field_9c = &data_80173c9c;
        o->field_90 = (void *)0x800fb100;
        o->pos_x = 0;
        o->pos_y = 0;
        o->field_09 = 7;
        o->field_45 = w;
        v = game_state.field_dc;
        game_state.field_64 = 0x90;
        o->field_7a = 0x20;
        o->field_7c = 0x1e0;
        data_80189454 = v;
        o->field_0d = 0;
        if (game_state.field_6b == 6) {
            o->field_04 = 4;
        }
        return 1;
    }
    return 0;
}
