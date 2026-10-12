/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void func_80144120(Object *object) {
    if (*(u8 *)&object->step_flags == 0) {
        func_80144220(object);
        object->countdown = (s16)object->countdown - 1;
        if (object->countdown & 0x80) {
            object->pos_x = 0xc0;
            object->state_2 = object->state_2 + 1;
            play_sound_at(0, 0, 0x20a);
        }
    }
}

void func_80144198(Object *object) {
    if ((s16)object->step_flags < 0) {
        func_80144e90(object);
    }
}
