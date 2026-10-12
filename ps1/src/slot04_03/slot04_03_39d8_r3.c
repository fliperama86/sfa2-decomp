/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void flag_parent_and_move_object_slot04_03(Object *obj) {
    ref_other.p = obj->parent;
    ref_other.p->field_225 = 1;
    update_object_motion_unless_paused(obj);
}
