/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ce2c0_slot05_06(Object *obj);
extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;
extern u8 data_801dd600_slot05_06[];
extern s32 data_801dd620_slot05_06[];

void func_801ce1dc_slot05_06(Object *obj) {
    obj->field_09 = 1;
    ((Slot04aObj *)obj)->field_47 = 0x18;
    ((Slot04aObj *)obj)->field_70 -= 0x10;
    func_801ce2c0_slot05_06(obj);
}

void func_801ce214_slot05_06(Object *obj) {
    SequenceStep **table;
    u8 a = obj->field_03 + 0x11;

    if (obj->field_66 == 0) {
        table = data_1f8000b4;
    } else {
        table = data_1f800164;
    }
    func_80130768(obj, a, table);
}

void func_801ce260_slot05_06(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    s16 dx = *(u16 *)((u8 *)data_801dd600_slot05_06 + ((obj->field_03 << 2) & 0x1fc));
    s16 dy = *(u16 *)((u8 *)data_801dd600_slot05_06 + ((obj->field_03 << 2) & 0x1fc) + 2);

    if (obj->field_0b != 0) {
        dx = -dx;
    }
    obj->pos_x += dx;
    obj->pos_y -= dy;
}

void func_801ce2c0_slot05_06(Object *obj) {
    s32 *p = (s32 *)((u8 *)data_801dd620_slot05_06 + ((obj->field_03 << 4) & 0x3f0));

    obj->field_4c = *p++;
    obj->field_54 = *p++;
    obj->field_50 = p[0];
    obj->field_58 = p[1];
    if (obj->field_0b != 0) {
        obj->field_4c = -obj->field_4c;
        obj->field_54 = -obj->field_54;
    }
}
