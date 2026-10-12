/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3248_slot04_0a(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        play_sound_at(obj, obj->side, 0x320);
        func_801307e0(obj, 0x1b);
    }
}

void func_801b32b4_slot04_0a(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->active_flag_3 = 0;
        func_801312b8(obj);
    }
}
