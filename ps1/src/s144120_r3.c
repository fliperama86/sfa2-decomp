/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void set_state_2_7_and_sequence_d(Object *object) {
    object->state_2 = 7;
    func_80144ee4(object, 0xd);
}

void func_801444b4(Object *object) {
    if (*(u8 *)&object->step_flags != 0) {
        object->state_2 = object->state_2 + 1;
        object->step_flags = object->step_flags & 0xff00;
        play_sound_at(0, 0, 0x20c);
    }
}
