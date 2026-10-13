/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void spawn_six_objects_on_first_state(Object *object) {
    int i;
    Object *o;
    if (object->state_2 == 0) {
        screen_state->step_index_2++;
        screen_state->countdown_timer = 0x20;
        object->ot_depth = 0;
        object->linked_object_2 = 0;
        object->phase_state = 0;
        for (i = 0; i < 2; i++) {
            o = (Object *)pop_block_from_pool();
            if (o != 0) {
                o->active_count = 1;
                o->field_02 = 0x31;
                o->table_index = i;
            }
        }
        for (i = 0; i < 2; i++) {
            o = (Object *)pop_block_from_pool();
            if (o != 0) {
                o->active_count = 1;
                o->field_02 = 0x32;
                o->table_index = 0;
                o->variant_index = i;
                if (i == 0) {
                    o->parent = &player_left;
                } else {
                    o->parent = &player_right;
                }
            }
        }
        for (i = 0; i < 2; i++) {
            o = (Object *)pop_block_from_pool();
            if (o != 0) {
                o->active_count = 1;
                o->field_02 = 0x17;
                o->table_index = 1;
                o->variant_index = i;
                if (i == 0) {
                    o->parent = &player_left;
                } else {
                    o->parent = &player_right;
                }
            }
        }
    }
}
