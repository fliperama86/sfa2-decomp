/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_air_state_pick_sequence_slot04_02(Object *obj);
void set_object_states_0_to_3_2_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b68b4_slot04_02(Object *obj) {
    int d;

    if (obj->active_state != 0) {
        if (obj->hit_flag_a != 0) {
            if ((s16)obj->meter >= 0x30 && obj->linked_object_3 == 0) {
                set_object_states_0_to_3_2_slot04_02(obj, 1, 0, 8, 0);
                obj->field_15a = 9;
                obj->active_flag_3 = 1;
                if (obj->side != 0) {
                    scratch_call_right(obj);
                } else {
                    scratch_call_left(obj);
                }
                return;
            }
        } else if (obj->input_dir_second != 0) {
            set_object_states_0_to_3_2_slot04_02(obj, 1, 0, 7, 0);
            obj->field_15a = 3;
            obj->active_flag_3 = 1;
            if (obj->side != 0) {
                scratch_call_right(obj);
            } else {
                scratch_call_left(obj);
            }
            return;
        } else if (obj->linked_object_3 == 0 && (d = ((Slot04bObj *)obj)->ground_y - 0x30, obj->pos_y < (s16)d)) {
            set_object_states_0_to_3_2_slot04_02(obj, 1, 0, 7, 0);
            obj->field_15a = 4;
            obj->active_flag_3 = 1;
            if (obj->side != 0) {
                scratch_call_right(obj);
            } else {
                scratch_call_left(obj);
            }
            return;
        }
    }
    obj->limit_lo = (obj->limit_lo & 0xfe) | 0x40;
    start_air_state_pick_sequence_slot04_02(obj);
}
