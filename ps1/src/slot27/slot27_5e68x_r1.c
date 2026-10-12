/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80028864_slot27[];
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];

void func_80015e68_slot27(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Object *a;
    Object *b;
    int i;
    u8 *base;
    int j;
    int m;
    int n;
    int v9;
    u16 t12;

    obj->state_0 = 2;
    for (m = 0; m < 0x160; m++) {
        data_801a2824[m] = ((u16 *)0x800edc00)[m];
        data_801a2824[m + 0xa00] = ((u16 *)0x800edc00)[m];
    }
    func_80137220(0, 0);
    i = 0;
    base = (u8 *)&game_state.field_54;
    if (*(s16 *)base >= 0) {
    do {
        a = (Object *)func_8011f1e0();
        if (a != 0) {
            a->field_02 = 0xaa;
            a->field_00 = 1;
            a->field_01 = 1;
            a->field_03 = 0x80;
            v9 = *(u8 *)&game_state.field_54 - i + 3;
            a->state_0 = 1;
            a->field_46 = 0x1f;
            a->ot_depth = v9;
            t12 = ((Slot27Obj *)obj)->field_12;
            a->pos_y = 0xbc;
            a->health = i << 5;
            a->step_flags = 0x14;
            a->field_98 = data_80017c28_slot27;
            a->field_9c = data_8001aa14_slot27;
            a->spawn_y = 0x1e0;
            a->field_90 = (void *)0x80038000;
            a->spawn_x = 0;
            a->field_0d = 0x16;
            ((Slot27Obj *)a)->field_12 = t12;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_02 = 0xaa;
            b->field_00 = 1;
            b->field_01 = 1;
            b->field_03 = 0x80;
            b->ot_depth = a->ot_depth - 1;
            b->state_0 = 1;
            b->field_46 = a->field_46;
            ((Slot27Obj *)b)->field_12 = ((Slot27Obj *)a)->field_12 + 2;
            b->pos_y = *(u16 *)&a->pos_y - 2;
            b->health = a->health;
            b->step_flags = (&game_state.field_11f)[i + 1];
            b->field_90 = (void *)0x80038000;
            b->field_98 = data_80017c28_slot27;
            b->field_9c = data_8001aa14_slot27;
            b->spawn_y = 0x1e0;
            b->spawn_x = 0;
            b->field_0d = 0;
            if (i != 7) {
                if (i == *(s16 *)&game_state.field_54) {
                    b->field_0d = (&game_state.field_11f)[i + 1] + 2;
                } else {
                    for (m = 0; m < 0x10; m++) {
                        data_801a2824[(&game_state.field_11f)[i + 1] * 0x10 + m] = *(u16 *)((u8 *)0x800ee000 + (&game_state.field_11f)[i + 1] * 0x20 + m * 2);
                        data_801a3c24[(&game_state.field_11f)[i + 1] * 0x10 + m] = *(u16 *)((u8 *)0x800ee000 + (&game_state.field_11f)[i + 1] * 0x20 + m * 2);
                    }
                    func_80137220(0, 0);
                    b->field_0d = (&game_state.field_11f)[i + 1] + 2;
                    a->field_0d = 0x15;
                    a->step_flags = 0x15;
                }
            } else {
                b->field_0d = (&game_state.field_11f)[8] + 2;
            }
            func_80130768(a, (s16)a->step_flags, data_80028864_slot27);
            func_80130768(b, (&game_state.field_11f)[i + 1], data_80028864_slot27);
        }
        i++;
    } while (i <= *(s16 *)&game_state.field_54);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->field_00 = 1;
        b->field_01 = 1;
        b->field_02 = 0xaa;
        b->field_03 = 0x80;
        b->ot_depth = 2;
        b->state_0 = 1;
        b->field_46 = a->field_46;
        ((Slot27Obj *)b)->field_12 = ((Slot27Obj *)a)->field_12 + 2;
        b->pos_y = *(u16 *)&a->pos_y - 2;
        b->health = a->health;
        b->field_90 = (void *)0x80038000;
        b->field_98 = data_80017c28_slot27;
        b->field_9c = data_8001aa14_slot27;
        b->spawn_y = 0x1e0;
        b->spawn_x = 0;
        b->field_0d = 0x16;
        func_80130768(b, 0x16, data_80028864_slot27);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->field_00 = 1;
        b->field_01 = 1;
        b->field_02 = 0xaa;
        b->field_03 = 0x80;
        b->ot_depth = 2;
        b->state_0 = 1;
        b->field_46 = a->field_46;
        ((Slot27Obj *)b)->field_12 = ((Slot27Obj *)a)->field_12 + 2;
        b->pos_y = *(u16 *)&a->pos_y - 2;
        b->health = a->health;
        b->field_90 = (void *)0x80038000;
        b->field_98 = data_80017c28_slot27;
        b->field_9c = data_8001aa14_slot27;
        b->spawn_y = 0x1e0;
        b->spawn_x = 0;
        b->field_0d = 0x17;
        func_80130768(b, (&game_state.field_11f)[i] + 0x17, data_80028864_slot27);
    }

    j = i;
    for (; j < 8; j++) {
        a = (Object *)func_8011f1e0();
        if (a != 0) {
            a->field_02 = 0xaa;
            a->field_00 = 1;
            a->field_01 = 1;
            a->field_03 = 0x80;
            v9 = j - *(u8 *)&game_state.field_54 + 3;
            a->state_0 = 1;
            a->field_46 = 0x1f;
            a->ot_depth = v9;
            t12 = ((Slot27Obj *)obj)->field_12;
            a->pos_y = 0xbc;
            a->health = j << 5;
            a->step_flags = 0x14;
            a->field_90 = (void *)0x80038000;
            a->field_98 = data_80017c28_slot27;
            a->field_9c = data_8001aa14_slot27;
            a->spawn_y = 0x1e0;
            a->spawn_x = 0;
            a->field_0d = 0x16;
            ((Slot27Obj *)a)->field_12 = t12;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_02 = 0xaa;
            b->field_00 = 1;
            b->field_01 = 1;
            b->field_03 = 0x80;
            b->ot_depth = a->ot_depth;
            b->state_0 = 1;
            b->field_46 = a->field_46;
            ((Slot27Obj *)b)->field_12 = ((Slot27Obj *)a)->field_12 + 2;
            b->pos_y = *(u16 *)&a->pos_y - 2;
            b->health = a->health;
            b->step_flags = (&game_state.field_11f)[j + 1];
            b->field_90 = (void *)0x80038000;
            b->field_98 = data_80017c28_slot27;
            b->field_9c = data_8001aa14_slot27;
            b->spawn_x = 0;
            b->spawn_y = 0x1e0;
            b->field_0d = 0;
            if (((game_state.field_50 >> (&game_state.field_11f)[j + 1]) & 1) && j < 7) {
                for (n = 0; n < 0x10; n++) {
                    data_801a2824[(&game_state.field_11f)[j + 1] * 0x10 + n] = *(u16 *)((u8 *)0x800ee000 + (&game_state.field_11f)[j + 1] * 0x20 + n * 2);
                    data_801a2824[(&game_state.field_11f)[j + 1] * 0x10 + n + 0xa00] = *(u16 *)((u8 *)0x800ee000 + (&game_state.field_11f)[j + 1] * 0x20 + n * 2);
                }
                func_80137220(0, 0);
                func_80130768(b, (&game_state.field_11f)[j + 1], data_80028864_slot27);
                b->field_0d = (&game_state.field_11f)[j + 1] + 2;
                a->field_0d = 0x15;
                a->step_flags = 0x15;
            } else {
                for (n = 0; n < 0x10; n++) {
                    data_801a2824[(&game_state.field_11f)[j + 1] * 0x10 + n] = *(u16 *)((u8 *)0x800edc00 + (&game_state.field_11f)[j + 1] * 0x20 + n * 2);
                    data_801a2824[(&game_state.field_11f)[j + 1] * 0x10 + n + 0xa00] = *(u16 *)((u8 *)0x800edc00 + (&game_state.field_11f)[j + 1] * 0x20 + n * 2);
                }
                func_80137220(0, 0);
                func_80130768(b, 0x13, data_80028864_slot27);
                b->field_0d = 0x15;
            }
            func_80130768(a, (s16)a->step_flags, data_80028864_slot27);
            b->field_98 = data_80017c28_slot27;
            b->field_90 = (void *)0x80038000;
            b->field_9c = data_8001aa14_slot27;
        }
    }
}
