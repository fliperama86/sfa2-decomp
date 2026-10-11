/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80144364(Object *object) {
    if (*(u8 *)&object->step_flags != 0) {
        object->state_2 = object->state_2 + 1;
        object->step_flags = object->step_flags & 0xff00;
        play_sound_at(0, 0, 0x20b);
    }
}
