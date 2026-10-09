/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801bfc70_slot04_00[];

void func_801b2ef8_slot04_00(Object *obj) {
    u16 t = obj->field_3a;
    u32 k = (t & 0x7f00) << 16;

    if (k != 0) {
        obj->field_3a = t & 0x80ff;
        func_801204f4(obj, obj->side, *(s16 *)((u8 *)data_801bfc70_slot04_00 + (k >> 23)));
    }
}
