/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void spawn_position_table_objects_and_list_objects(void) {
    s16 v;
    int t;
    if (data_80197f10 >= 6) {
        t = game_state.field_48 * 2 - 1;
        v = game_state.count_index;
        if (v != t) {
            spawn_object_with_position_table(0, 0);
            spawn_object_with_position_table(1, (u8)(v + 2));
        } else {
            spawn_object_with_position_table(0, 1);
        }
        spawn_object_with_position_table(9, 0);
        spawn_object_with_position_table(10, 0);
    }
    spawn_objects_from_table();
}
