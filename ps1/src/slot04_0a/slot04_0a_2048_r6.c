/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void load_velocities_from_table_5_slot04_0a(Object *obj);

void update_sequence_land_and_reset_on_end_slot04_0a(Object *obj) {
    s16 t;
    u16 u;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff00) == 0x100) {
        obj->step_flags = t & 0xff;
        obj->facing_side = obj->saved_facing;
    }
    u = obj->step_flags;
    if ((u & 0xff) != 0) {
        obj->step_flags = u & 0xff00;
        load_velocities_from_table_5_slot04_0a(obj);
    }
    if ((s16)obj->step_flags & 0x8000) {
        obj->pos_y_frac = 0;
        obj->pos_y = (u16)obj->ground_y;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
