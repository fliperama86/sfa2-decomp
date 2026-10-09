/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c0404_slot04_0a[];

void func_801b3f7c_slot04_0a(Object *obj) {
    u16 t = obj->field_3a;
    u32 k = (t & 0x7f00) << 16;

    if (k != 0) {
        obj->field_3a = t & 0x80ff;
        func_801204f4(obj, obj->side, *(s16 *)((u8 *)data_801c0404_slot04_0a + (k >> 23)));
    }
}
