/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_x_by_facing_with_decel_slot04_03(Object *obj);

extern Slot04_03Rec0678 data_801c0678_slot04_03[];

void start_table_launch_on_end_flag_slot04_03(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[0x18];
    s16 t;
    Object *o;

    update_sequence(obj);
    t = obj->step_flags;
    if (!(t & 0x8000)) {
        if ((u8)t == 0) {
            move_x_by_facing_with_decel_slot04_03(obj);
        }
    } else {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 5);
        play_side_sound_at(obj, obj->side, 0xd);
        obj->vel_x = data_801c0678_slot04_03[obj->direction_index >> 1].vel_x;
        o = obj;
        o->accel_x = data_801c0678_slot04_03[obj->direction_index >> 1].accel_x;
        o->vel_y = data_801c0678_slot04_03[obj->direction_index >> 1].vel_y;
        o->accel_y = data_801c0678_slot04_03[o->direction_index >> 1].accel_y;
        t = (o->direction_index >> 1) + 0x35;
        set_sequence_by_side_and_mode(o, t);
    }
}
