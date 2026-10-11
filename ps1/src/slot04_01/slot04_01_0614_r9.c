/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801bede4_slot04_01[];

void func_801b3b4c_slot04_01(Object *obj) {
    u16 t = obj->step_flags;
    u32 k = (t & 0x7f00) << 16;

    if (k != 0) {
        obj->step_flags = t & 0x80ff;
        play_side_sound_at(obj, obj->side, *(s16 *)((u8 *)data_801bede4_slot04_01 + (k >> 23)));
    }
}
