/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f58_slot04_0e(Object *obj);
void apply_velocity_and_accel_to_position_slot04_0e(Object *obj);

void update_arc_land_on_wall_push_check_slot04_0e(Object *o) {
    /* Stand-in, not recovered source: the one 8-bit local a, which holds the field, then the facing, then the sequence number, reproduces the original's choice of registers. Without it this function differs from the original in 9 instruction slots. */
    /* The figure is for each of the three values in a u8 local of its own. Only the sequence number in a local of its own: 3 slots. The one local as an int: 2 slots; without the (s8) cast below: 7. */
    u8 a;

    apply_velocity_and_accel_to_position_slot04_0e(o);
    if (o->ground_y < o->pos_y) {
        func_801b2f58_slot04_0e(o);
    } else if ((s16)(o->ground_y - 0x30) < o->pos_y) {
        update_sequence(o);
    } else {
        a = o->facing_side;
        if (a == 0) {
            a = 2;
        } else {
            a = 1;
        }
        if (o->bound_push_flag != (s8)a) {
            update_sequence(o);
        } else {
            a = 0x60;
            o->state_3++;
            o->vel_y = 0;
            o->accel_y = 0;
            if (o->field_49 == 0) {
                a = 0x2e;
            }
            set_sequence_by_side_and_mode(o, a);
            play_object_sound_by_kind(o);
        }
    }
}
