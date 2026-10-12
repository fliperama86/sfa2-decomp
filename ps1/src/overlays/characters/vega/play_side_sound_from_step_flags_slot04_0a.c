/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s16 data_801c0404_slot04_0a[];

void play_side_sound_from_step_flags_slot04_0a(Object *obj) {
    u16 t = obj->step_flags;
    u32 k = (t & 0x7f00) << 16;

    if (k != 0) {
        obj->step_flags = t & 0x80ff;
        play_side_sound_at(obj, obj->side, *(s16 *)((u8 *)data_801c0404_slot04_0a + (k >> 23)));
    }
}
