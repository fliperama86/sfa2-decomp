/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801befb0_slot04_01[];

void call_handler_by_state_3_clear_flag_near_ground_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    data_801befb0_slot04_01[o->state_3](o);
    if (obj->field_1a5 != 0) {
        if (o->vel_y < 0) {
            if (o->pos_y - o->ground_y >= -0x1f) {
                obj->field_1a5 = 0;
            }
        }
    }
}
