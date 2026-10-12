/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void spawn_offset_effect_and_play_sounds(Object *object) {
    Object *spawned = spawn_effect_variant_12_at_object(object, 6);
    if (spawned) {
        if (object->facing_side != 0) {
            spawned->pos_x = spawned->pos_x + table_8017ca64[object->kind].first;
        } else {
            spawned->pos_x = spawned->pos_x - table_8017ca64[object->kind].first;
        }
        spawned->pos_y = spawned->pos_y - table_8017ca64[object->kind].second;
    }
    play_sound_at(object, object->side, 0x313);
    if (table_8017ca38[object->kind] != -1) {
        play_side_sound_at(object, object->side, table_8017ca38[object->kind]);
    }
}

Object *spawn_effect_variant_12_at_object(Object *object, int unused) {
    ref_other.p = (Object *)pop_block_from_pool();
    if (ref_other.p != 0) {
        ref_other.p->active_count = 1;
        ref_other.p->field_02 = 5;
        ref_other.p->table_index = 0;
        ref_other.p->field_0e = object->field_0e;
        ref_other.p->ot_depth = 4;
        ref_other.p->variant_index = 0x12;
        ref_other.p->pos_x = object->pos_x;
        ref_other.p->pos_y = object->pos_y;
        ref_other.p->facing_side = object->facing_side;
        ref_other.p->field_1c = object->field_1c;
        ref_other.p->field_0c = 0;
        return ref_other.p;
    } else {
        return 0;
    }
}
