/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_slide_with_sound_when_mode_set_slot04_09(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    if (obj->mode != 0) {
        o->state_3++;
        play_sound_at(o, o->side, 0x324);
        if (o->facing_side != 0) {
            o->vel_x = 0xd0000;
            o->accel_x = -0xd000;
        } else {
            o->vel_x = 0xfff30000;
            o->accel_x = 0xd000;
        }
    }
    update_sequence(o);
}
