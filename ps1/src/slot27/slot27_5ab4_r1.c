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
    obj->data_ptr_a = data_80017c28_slot27;
    obj->data_ptr_b = data_8001aa14_slot27;
    obj->field_90 = (void *)0x80038000;
    obj->facing_side = 0;
    obj->state_0++;
    if (obj->field_60 != 0) {
        obj->pos_x = data_80028b38_slot27[obj->table_index].first + data_80055f30;
        obj->pos_y = -data_80028b38_slot27[obj->table_index].second + data_80055f34 + 0x200;
        func_80130768(obj, 0, table_80028a4c_slot27);
        obj->ot_depth = 8;
        obj->spawn_x = 0;
        obj->spawn_y = 0x1e0;
        obj->image_row = 0;
        for (i = 0; i < 16; i++) {
            data_801a2b84[i] = src[i];
            data_801a2b84[i + 0xa00] = src[i];
        }
        func_80137220(0, 0);
    } else {
        obj->pos_x = data_80028abc_slot27[obj->table_index].first + data_80055f30;
        obj->pos_y = -data_80028abc_slot27[obj->table_index].second + data_80055f34 + 0x230;
        func_80130768(obj, data_80028b0c_slot27[obj->table_index], data_800289b4_slot27);
        obj->ot_depth = 9;
        obj->spawn_x = 0x20;
        obj->spawn_y = 0x1e0;
        obj->image_row = 0x16;
        func_800167d0_slot27(obj, (s16 *)data_80028a74_slot27);
    }
}
