/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);
void update_motion_or_land_slot04_07(Object *obj);

void update_or_launch_other_by_mode_slot04_07(Object *obj) {
    if (((Slot04aObj *)obj)->mode == 0) {
        update_sequence(obj);
    } else {
        obj->vel_y = 0x40000;
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 5, -0x200, 0, 1, 0);
        update_motion_or_land_slot04_07(obj);
    }
}

void update_motion_or_land_slot04_07(Object *obj) {
    if (func_801b41c4_slot04_07(obj) >= 0 || obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        func_80131638(obj);
    }
}
