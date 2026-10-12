/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int apply_velocity_and_acceleration_slot04_17(Object *obj);

void update_air_land_or_active_flag_sequence_slot04_17(Object *obj) {
    s16 t;
    int a;
    u8 s;

    if (apply_velocity_and_acceleration_slot04_17(obj) < 0) {
        t = obj->ground_y;
        if (obj->pos_y >= t) {
            obj->pos_y = t;
            play_object_sound_by_kind(obj);
            a = 0x37;
            s = obj->variant_index;
            obj->pos_y_frac = 0;
            obj->active_state = 0;
            obj->state_3 = 8;
            obj->active_flag_4 = 0;
            if (!(s & 0x80)) {
                a = (s & 0x7f) + 0x38;
            }
            set_sequence_by_side_and_mode(obj, a);
        }
    }
    if (obj->active_flag != 0) {
        a = obj->variant_index + 0x35;
        obj->active_flag = 0;
        obj->variant_index |= 0x80;
        set_sequence_by_side_and_mode(obj, a);
    } else {
        update_sequence(obj);
    }
}
