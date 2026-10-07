/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_800efd30_slot0f[];
extern SequenceStep *data_800efd84_slot0f[];
extern SequenceStep *data_800efd88_slot0f[];
extern Slot12Prim data_800f77b4_slot0f[];
extern Slot12Prim data_800f78f4_slot0f[];
extern Slot12Prim data_800f7a34_slot0f[];
void func_800e5d18_slot0f(Object *obj, Slot12Prim *a, int b, int c);

void func_800e64b0_slot0f(Object *obj) {
    FrameRecord *rec = table_8016e614;

    if (game_state.field_2bd == 0) {
        rec = table_8016e5c4;
    }
    obj->field_01 = 0;
    obj->field_04++;
    rec += ((Slot0fObj *)obj)->field_5c;
    switch (obj->field_48) {
    case 0:
        func_80130768(obj, rec->field_08, data_800efd30_slot0f);
        func_800e5d18_slot0f(obj, (Slot12Prim *)((u8 *)data_800f77b4_slot0f + obj->field_03 * 64), 0x20, 0x20);
        break;
    case 1:
        func_80130768(obj, 0, data_800efd84_slot0f);
        func_800e5d18_slot0f(obj, (Slot12Prim *)((u8 *)data_800f78f4_slot0f + obj->field_03 * 64), 0x20, 0x20);
        break;
    case 2:
        func_80130768(obj, 0, data_800efd88_slot0f);
        func_800e5d18_slot0f(obj, (Slot12Prim *)((u8 *)data_800f7a34_slot0f + obj->field_03 * 64), 0x20, 0x10);
        break;
    }
    obj->pos_x = obj->pos_x + 8;
    obj->pos_y = obj->pos_y + 0x10;
    if (obj->field_48 == 2 && obj->field_03 == 0) {
        obj->pos_x = 0x30;
        obj->pos_y = 0x70;
    }
}
