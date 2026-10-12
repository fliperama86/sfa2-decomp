/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c64f4_slot04_02[];
extern u16 data_801c64f8_slot04_02[];

void nudge_other_x_by_table_every_fourth_tick_slot04_02(Object *obj) {
    int i;

    if ((game_state.tick_count & 3) == 0) {
        i = 1;
        if ((u8)obj->step_flags != 0) {
            i = 2;
        }
        for (; i >= 0; i--) {
            u16 a;

            if (!func_80148e84(obj)) {
                break;
            }
            if ((u8)obj->step_flags != 0) {
                a = data_801c64f8_slot04_02[i];
            } else {
                a = data_801c64f4_slot04_02[i];
            }
            if (obj->facing_side != 0) {
                a = -a;
            }
            ref_other.p->pos_x = a + ref_other.p->pos_x;
        }
    }
}

void update_sequence_or_reset_state_variant_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}
