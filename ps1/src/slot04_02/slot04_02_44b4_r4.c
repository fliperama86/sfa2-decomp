/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_high_hop_from_other_box_position_slot04_02(Object *obj) {
    Object *p;

    obj->vel_y = 0x58000;
    obj->accel_y = -0x1800;
    p = obj->other;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x20000;
    } else {
        obj->vel_x = -0x20000;
    }
    obj->accel_x = 0;
    obj->state_3 = 0xc;
    add_to_meter_clamped(obj, 8);
    play_sound_at(p, p->side, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x57);
    set_position_from_other_box(obj);
}
