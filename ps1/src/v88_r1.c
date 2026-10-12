/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Every path ends in its own call of update_air_move_and_land. The compiler merges only
   the call instruction, which is what the original shows: the argument
   register is reloaded on the paths that come after the first call and left
   alone on the path that comes straight from the entry. */
void func_8012dee4(Object *object) {
    u8 v = object->bound_push_flag;
    object->step_applied_flag = 0;
    if (v == 0) {
        update_air_move_and_land(object);
        return;
    }
    if (apply_table_step_to_pos_x(object) == 0) {
        update_air_move_and_land(object);
        return;
    }
    object->state_3++;
    update_air_move_and_land(object);
}
