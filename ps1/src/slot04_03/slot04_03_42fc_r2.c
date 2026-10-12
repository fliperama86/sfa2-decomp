/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c0a08_slot04_03[];

void func_801b445c_slot04_03(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (t & 0xff) {
            if (consume_active_frame_flag(obj) & 0xff) {
                obj->state_3 = 0;
            }
        }
        update_sequence(obj);
    }
}

void move_x_by_table_by_step_value_facing_slot04_03(Object *obj) {
    s16 t = obj->step_flags;
    s16 a;
    int v;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        a = t & 0xff;
        if (a == (s16)obj->countdown) {
            if ((t & 0xff00) == 0) {
                update_sequence(obj);
                return;
            }
        } else {
            obj->countdown = a;
        }
        v = *(s32 *)((u8 *)data_801c0a08_slot04_03 + (a & 0xfc));
        if (obj->facing_side == 0) {
            v = -v;
        }
        *(s32 *)&obj->pos_x_frac += v;
        update_sequence(obj);
    }
}
