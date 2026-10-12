/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801218fc(Object *object) {
    int i;
    Object *o;
    if (object->state_2 == 0) {
        data_8018f5a0->step_index_2++;
        data_8018f5a0->countdown_timer = 0x20;
        object->ot_depth = 0;
        object->linked_object_2 = 0;
        object->phase_state = 0;
        for (i = 0; i < 2; i++) {
            o = (Object *)func_8011f1e0();
            if (o != 0) {
                o->active_count = 1;
                o->field_02 = 0x31;
                o->table_index = i;
            }
        }
        for (i = 0; i < 2; i++) {
            o = (Object *)func_8011f1e0();
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
            o = (Object *)func_8011f1e0();
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
