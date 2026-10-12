/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c06ac_slot04_0a[];
void apply_velocity_and_acceleration_slot04_0a(Object *obj);

void update_airborne_until_ground_then_bounce_slot04_0a(Object *obj) {
    int k;
    int b;
    u8 f;
    update_sequence(obj);
    k = -1;
    if (obj->field_4b != 0) {
        k = 1;
    }
    obj->field_165 = k;
    f = ((Slot04aObj *)obj)->mode;
    if (f == 0) {
        obj->field_165 = 0;
        if (((s16)obj->step_flags & 0xff00) != 0x100) {
            apply_velocity_and_acceleration_slot04_0a(obj);
            if (!(obj->pos_y < obj->ground_y)) {
                k = 0xa0000;
                b = -0x1000;
                obj->vel_y = 0x28000;
                obj->accel_y = -0x3000;
                obj->state_3++;
                if (obj->facing_side == 0) {
                    k = 0xfff60000;
                    b = 0x1000;
                }
                obj->vel_x = k;
                obj->accel_x = b;
                set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x33);
            }
        }
    } else if (f == 2) {
        k = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            k = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c06ac_slot04_0a[k];
    }
}
