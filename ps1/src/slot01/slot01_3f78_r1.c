/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];
extern SequenceStep *data_800240f8_slot01[];

void func_80013f78_slot01(Object *obj) {
    Rect rect;
    Object *a;
    Object *b;
    Object *c;
    int i;
    u8 *base;
    u16 t12;
    int v9;
    obj->field_04 = 2;
    base = (u8 *)&game_state.field_54;
    i = 0;
    if (*(s16 *)base >= 0) {
    do {
        a = (Object *)func_8011f1e0();
        if (a != 0) {
            a->field_02 = 0x30;
            a->field_00 = 1;
            a->field_01 = 1;
            a->field_03 = 0x80;
            v9 = *(u8 *)&game_state.field_54 - i + 1;
            a->field_0d = 0;
            a->field_04 = 1;
            a->field_46 = 0x1f;
            a->field_09 = v9;
            t12 = obj->pos_x;
            a->pos_y = 0xbc;
            a->field_5c = i << 5;
            a->field_61 = 0x14;
            a->field_7a = 0x70;
            a->field_7c = 0x1f4;
            a->field_98 = data_80019330_slot01;
            a->field_90 = (void *)0x80059000;
            a->field_9c = data_800195b8_slot01;
            a->pos_x = t12;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x30;
            b->field_01 = 1;
            b->field_03 = 0x80;
            b->field_09 = a->field_09;
            b->field_04 = 1;
            b->field_0d = 0;
            b->field_46 = a->field_46;
            b->pos_x = a->pos_x + 2;
            b->pos_y = a->pos_y - 2;
            b->field_5c = a->field_5c;
            b->field_61 = (&game_state.field_11f)[i + 1];
            b->field_7a = 0x70;
            b->field_98 = data_80019330_slot01;
            b->field_9c = data_800195b8_slot01;
            b->field_90 = (void *)0x80059000;
            if (i != 7) {
                if (i == *(s16 *)&game_state.field_54) {
                    b->field_7c = (&game_state.field_11f)[i + 1] + 0x1e0;
                } else {
                    rect.x = b->field_7a;
                    rect.y = (&game_state.field_11f)[i + 1] + 0x1e0;
                    rect.w = 0x10;
                    rect.h = 1;
                    func_80157fc4(&rect, (u8 *)(0x80071408 + (((&game_state.field_11f)[i + 1] << 5))));
                    func_80157d9c(0);
                    b->field_7c = (&game_state.field_11f)[i + 1] + 0x1e0;
                    a->field_7c = 0x1f3;
                    a->field_61 = 0x15;
                }
            } else {
                b->field_7c = (&game_state.field_11f)[8] + 0x1e0;
            }
            func_80130768(a, a->field_61, data_800240f8_slot01);
            func_80130768(b, (&game_state.field_11f)[i + 1], data_800240f8_slot01);
        }
        i++;
    } while (i <= *(s16 *)&game_state.field_54);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->field_00 = 1;
        b->field_02 = 0x30;
        b->field_01 = 1;
        b->field_03 = 0x80;
        b->field_09 = 0;
        b->field_0d = 0;
        b->field_04 = 1;
        c = b;
        c->field_46 = a->field_46;
        c->pos_x = a->pos_x + 2;
        c->pos_y = a->pos_y - 2;
        c->field_5c = a->field_5c;
        c->field_7a = 0x70;
        c->field_7c = 0x1f4;
        c->field_98 = data_80019330_slot01;
        c->field_90 = (void *)0x80059000;
        c->field_9c = data_800195b8_slot01;
        func_80130768(c, 0x16, data_800240f8_slot01);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->field_00 = 1;
        b->field_02 = 0x30;
        b->field_01 = 1;
        b->field_03 = 0x80;
        b->field_09 = 0;
        b->field_0d = 0;
        b->field_04 = 1;
        b->field_46 = a->field_46;
        b->pos_x = a->pos_x + 2;
        c = b;
        c->pos_y = a->pos_y - 2;
        c->field_5c = a->field_5c;
        c->field_7a = 0x70;
        c->field_7c = 0x1f5;
        c->field_98 = data_80019330_slot01;
        c->field_90 = (void *)0x80059000;
        c->field_9c = data_800195b8_slot01;
        func_80130768(c, (&game_state.field_11f)[i] + 0x17, data_800240f8_slot01);
    }
    if (i < 8) {
        do {
            a = (Object *)func_8011f1e0();
            if (a != 0) {
                a->field_02 = 0x30;
                a->field_00 = 1;
                a->field_01 = 1;
                a->field_03 = 0x80;
                v9 = i - *(u8 *)&game_state.field_54 + 2;
                a->field_0d = 0;
                a->field_04 = 1;
                a->field_46 = 0x1f;
                a->field_09 = v9;
                t12 = obj->pos_x;
                a->pos_y = 0xbc;
                a->field_5c = i << 5;
                a->field_61 = 0x14;
                a->field_7a = 0x70;
                a->field_7c = 0x1f4;
                a->field_98 = data_80019330_slot01;
                a->field_90 = (void *)0x80059000;
                a->field_9c = data_800195b8_slot01;
                a->pos_x = t12;
            }
            b = (Object *)func_8011f1e0();
            if (b != 0) {
                b->field_00 = 1;
                b->field_02 = 0x30;
                b->field_01 = 1;
                b->field_03 = 0x80;
                b->field_09 = a->field_09;
                b->field_0d = 0;
                b->field_04 = 1;
                b->field_46 = a->field_46;
                b->pos_x = a->pos_x + 2;
                b->pos_y = a->pos_y - 2;
                b->field_5c = a->field_5c;
                b->field_3a = (&game_state.field_11f)[i + 1];
                b->field_7a = 0x70;
                if (((game_state.field_50 >> (&game_state.field_11f)[i + 1]) & 1) && i < 7) {
                    func_80130768(b, (&game_state.field_11f)[i + 1], data_800240f8_slot01);
                    rect.x = b->field_7a;
                    rect.y = (&game_state.field_11f)[i + 1] + 0x1e0;
                    rect.w = 0x10;
                    rect.h = 1;
                    func_80157fc4(&rect, (u8 *)(0x80071408 + (((&game_state.field_11f)[i + 1] << 5))));
                    func_80157d9c(0);
                    b->field_7c = (&game_state.field_11f)[i + 1] + 0x1e0;
                    a->field_7c = 0x1f3;
                    a->field_61 = 0x15;
                } else {
                    func_80130768(b, 0x13, data_800240f8_slot01);
                    b->field_7c = 0x1f3;
                }
                func_80130768(a, a->field_61, data_800240f8_slot01);
                b->field_90 = (void *)0x80059000;
                b->field_98 = data_80019330_slot01;
                b->field_9c = data_800195b8_slot01;
            }
            i++;
        } while (i < 8);
    }
}
