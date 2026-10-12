/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_round_timer_and_end_round(GameState *state) {
    Attacker *a = (Attacker *)state;
    int mode;
    if (a->skip_flag != 0) {
        a->field_6d = 0;
        a->field_2c = 0xff;
        a->field_4e = 0xff;
        return;
    }
    if ((a->field_30 == 0 || *(u16 *)data_801a6984 != 0) && (a->field_49 & 0x80) == 0 &&
        *(u16 *)&a->field_64 == 0 && a->mode == 0) {
        a->countdown--;
        if (a->countdown == 0) {
            a->countdown = 0x3c;
            if (a->field_49 == 0) {
                a->field_49 = 0;
                a->field_4d = 0xff;
                set_winner_by_health(a);
            } else {
                a->field_49--;
            }
        }
    }
    mode = a->mode;
    if (mode != 0) {
        if (mode == 3) {
            a->field_64 = 0;
            a->a6_flag = 1;
        } else {
            update_round_winner(a, mode);
            func_801229b4(a);
        }
        if (a->field_4d == 0) {
            a->timer_init = 0x3c;
            a->field_5d = 3;
            a->field_5e = 3;
        }
        data_8018f5a0->substep_index++;
        data_8018f5a0->step_index_2 = 0;
        a->field_ca = 0;
        a->active_flag = 1;
        a->active_flag_2 = 1;
        a->field_4e = 1;
        a->field_6d = 0;
        a->field_76 = 0;
        player_left.field_165 = 0;
        player_right.field_165 = 0;
        update_ec4_from_round_time_and_health(a);
        set_focus_kind_copy_from_object_b(a);
    }
}
