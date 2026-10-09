/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *table_80028a4c_slot27[];
extern Pair data_80028b38_slot27[];
extern Pair data_80028abc_slot27[];
extern s16 data_80028b0c_slot27[];
extern SequenceStep *data_800289b4_slot27[];
extern u16 data_80028a74_slot27[];
extern u16 data_80055f30;
extern u16 data_80055f34;
void func_800167d0_slot27(Object *obj, s16 *p);

void func_80015ab4_slot27(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    int i;
    u16 *src = (u16 *)0x800edb00;

    obj->field_01 = 1;
    obj->field_98 = data_80017c28_slot27;
    obj->field_9c = data_8001aa14_slot27;
    obj->field_90 = (void *)0x80038000;
    obj->field_0b = 0;
    obj->field_04++;
    if (obj->field_60 != 0) {
        obj->pos_x = data_80028b38_slot27[obj->field_03].first + data_80055f30;
        obj->pos_y = -data_80028b38_slot27[obj->field_03].second + data_80055f34 + 0x200;
        func_80130768(obj, 0, table_80028a4c_slot27);
        obj->field_09 = 8;
        obj->field_7a = 0;
        obj->field_7c = 0x1e0;
        obj->field_0d = 0;
        for (i = 0; i < 16; i++) {
            data_801a2b84[i] = src[i];
            data_801a2b84[i + 0xa00] = src[i];
        }
        func_80137220(0, 0);
    } else {
        obj->pos_x = data_80028abc_slot27[obj->field_03].first + data_80055f30;
        obj->pos_y = -data_80028abc_slot27[obj->field_03].second + data_80055f34 + 0x230;
        func_80130768(obj, data_80028b0c_slot27[obj->field_03], data_800289b4_slot27);
        obj->field_09 = 9;
        obj->field_7a = 0x20;
        obj->field_7c = 0x1e0;
        obj->field_0d = 0x16;
        func_800167d0_slot27(obj, (s16 *)data_80028a74_slot27);
    }
}
