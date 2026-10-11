/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012edac(Object *object) {
    int v;

    if ((s16)object->step_flags < 0 && (v = object->field_46 - 0x100, object->field_46 = v, (s16)v < 0)) {
        object->field_0b = object->field_158;
        object->field_15b = 0;
        object->field_247 = 0;
        object->field_166 = 0;
        func_801312b8(object);
    } else {
        if (object->field_12a != 0) {
            *(s32 *)&object->pos_x_frac += object->vel_x;
        }
        update_sequence(object);
    }
}

void func_8012ee3c(Object *object) {
    u16 old;

    old = object->field_46;
    object->field_46 = old - 1;
    if ((u8)old == 0) {
        object->state_2 = 1;
    }
    update_sequence(object);
}

void func_8012ee7c(Object *object) {
    Vec4 *table;

    table = vec_table_80170274;
    if (object->kind & 0xd40) {
        table = vec_table_80170514;
    }
    if (object->field_295 != 0) {
        table = vec_table_80170cf4;
        if (object->field_69 >= 2) {
            table = vec_table_80170f34;
        }
    }
    game_state.cursor = (u32 *)&table[object->field_63 & 0x3f];
    func_8012efa4(object, (Vec4 *)game_state.cursor);
    func_8012efcc(object, (Vec4 *)game_state.cursor);
}

void func_8012ef24(Object *object) {
    Vec4 *table;
    Vec4 *entry;
    int index;
    s32 a, b;

    index = 0;
    table = vec_table_80170a54;
    if (object->kind & 0xd40) {
        table = vec_table_801707b4;
    }
    if (object->field_295 == 0) {
        index = object->field_63 & 0x3f;
    }
    entry = (Vec4 *)((index << 4) + (int)table);
    a = entry->a;
    b = entry->b;
    if (object->field_72 == 0) {
        a = -a;
        b = -b;
    }
    object->vel_x = a;
    object->accel_x = b;
    object->vel_y = entry->c;
    object->accel_y = entry->d;
}

void func_8012efa4(Object *object, Vec4 *entry) {
    s32 a, b;

    a = entry->a;
    b = entry->b;
    if (object->field_72 == 0) {
        a = -a;
        b = -b;
    }
    object->vel_x = a;
    object->accel_x = b;
}

void func_8012efcc(Object *object, Vec4 *entry) {
    object->vel_y = entry->c;
    object->accel_y = entry->d;
}

int func_8012efe4(Object *object) {
    return (object->field_130 & 0xa000) == 0;
}
