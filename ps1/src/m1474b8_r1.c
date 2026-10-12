/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801474b8(Object *object) {
    object->state_0 = object->state_0 + 1;
    data_80189458 = 0;
    data_8018945c = 0;
    object->variant_index_2 = 1;
    object->offset_index = 0x10;
    object->step_count = 0;
    if (game_state.field_64 != 0) {
        play_side_sound_at(object->parent, object->parent->side, 0xd);
        set_cd_volume(2);
        object->field_01 = 1;
        func_801476f8(object);
        func_80130768(object, game_state.spawn_kind, seqs_8017cbac);
        func_801477ac(object, 1, 0x10);
    } else {
        object->state_0 = 2;
    }
}
