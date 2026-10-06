/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8002874c_slot12[];
extern SequenceStep *data_800287a0_slot12[];
extern SequenceStep *data_800287a4_slot12[];
extern u8 data_8002bfe8_slot12[];
extern u8 data_8002c128_slot12[];
extern u8 data_8002c268_slot12[];
void func_80014434_slot12(Object *obj, Slot12Prim *a, int b, int c);

void func_80016368_slot12(Object *obj) {
    FrameRecord *rec = table_8016e614;
    u8 t;

    if (game_state.field_2bd == 0) {
        rec = table_8016e5c4;
    }
    obj->field_01 = 0;
    obj->field_04++;
    rec += ((Slot12Obj *)obj)->field_5c;
    switch (obj->field_48) {
    case 0:
        func_80130768(obj, rec->field_08, data_8002874c_slot12);
        func_80014434_slot12(obj, (Slot12Prim *)(data_8002bfe8_slot12 + obj->field_03 * 64), 0x20, 0x20);
        break;
    case 1:
        func_80130768(obj, 0, data_800287a0_slot12);
        func_80014434_slot12(obj, (Slot12Prim *)(data_8002c128_slot12 + obj->field_03 * 64), 0x20, 0x20);
        break;
    case 2:
        func_80130768(obj, 0, data_800287a4_slot12);
        func_80014434_slot12(obj, (Slot12Prim *)(data_8002c268_slot12 + obj->field_03 * 64), 0x20, 0x10);
        break;
    }
    obj->pos_x = obj->pos_x + 8;
    obj->pos_y = obj->pos_y + 0x10;
    if ((obj->field_48 == 2) & (obj->field_03 == 0)) {
        obj->pos_x = 0x30;
        obj->pos_y = 0x70;
    }
}
