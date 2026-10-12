/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012f384(Object *object) {
    u8 flag;

    object->state_3 = 5;
    flag = object->bound_push_flag != 1;
    play_sound_at(object, object->side, 0x314);
    object->facing_side = flag;
    set_sequence_by_side(object, 0x30);
}

void func_8012f3e0(Object *object) {
    object->state_3 = 4;
    set_sequence_by_side(object, 0x1c);
}
