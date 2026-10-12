/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void halve_vel_x_add_meter_set_sequence_slot2b(Object *obj);

/* What it does (inferred, not an original name): sets state_3 to 3 and
   active_flag_3 to 1. When attack_param_d is not 0 it calls halve_vel_x_add_meter_set_sequence_slot2b
   and ends. Otherwise it takes half of direction_index, passes it to
   add_to_meter_clamped, and then calls set_sequence_by_side_and_mode with that half plus a base,
   0xc when variant_index is 0 and 0x12 otherwise, plus 3 when input_dir_second is not
   0, truncated to 16 bits. */
void set_state_3_and_start_direction_sequence_slot2b(Object *obj) {
    /* Stand-in, not recovered source: the local t, which holds the half and is then used again for the base, reproduces the original's choice of registers. Without it this function differs from the original in 5 instruction slots. */
    /* The figure is for a local of its own for the base, as int; as u16 it is 8. Inferred: the original keeps the half before the first call and the base after it in the same register (v1), and that register is not the second argument's, which a value set only after the first call would take. */
    u16 t;
    u16 half;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    if (obj->attack_param_d != 0) {
        halve_vel_x_add_meter_set_sequence_slot2b(obj);
    } else {
        t = obj->direction_index >> 1;
        half = t;
        add_to_meter_clamped(obj, t);
        t = obj->variant_index == 0 ? 0xc : 0x12;
        if (obj->input_dir_second != 0) {
            t += 3;
        }
        set_sequence_by_side_and_mode(obj, (u16)(t + half));
    }
}
