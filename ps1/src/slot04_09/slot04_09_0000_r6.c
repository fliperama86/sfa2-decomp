/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_child_table_index_eighteen_slot04_09(Object *obj);

void update_sequence_spawn_child_on_flag_2_slot04_09(Object *obj) {
    u16 t = obj->step_flags;

    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
    }
    t = obj->step_flags;
    if ((t & 0xff) == 2) {
        obj->step_flags = t & 0xff00;
        spawn_child_table_index_eighteen_slot04_09(obj);
    }
    update_sequence(obj);
}
