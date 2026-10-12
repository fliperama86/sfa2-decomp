/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80143e60(Object *object) {
    table_8017b5c4[object->state_1](object);
}

void func_80143ea0(Object *object) {
    table_8017b5d4[object->state_2](object);
    advance_sequence_checked(object);
    func_80120028(object);
}

void func_80143efc(Object *object) {
    object->pos_x = 0xc0;
    object->pos_y = 0x70;
    object->state_2 = object->state_2 + 1;
    func_80144ee4(object, 0xf);
    func_80143f4c(object);
}

void func_80143f4c(Object *object) {
    if ((u8)object->step_flags) {
        object->state_2 = object->state_2 + 1;
        object->step_flags = object->step_flags & 0xff00;
        play_sound_at(0, 0, 0x207);
    }
}
