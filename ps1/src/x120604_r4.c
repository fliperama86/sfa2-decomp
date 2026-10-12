/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80121f74(GameState *g) {
    int x;
    if (((g->versus_flag == 0 && g->field_226 == 0) ? g->leader_bits : (g->leader_bits || g->field_226)) == 0) {
        func_80123e34();
    }
    if (g->field_31 != 0) {
        if (g->field_74 == 0) {
            func_80154584();
        }
    } else if (g->versus_flag != 0 && data_80197f1c != 0) {
        if (g->field_74 == 0) {
            func_801241d8();
        }
        func_80138164();
        func_801510bc();
        func_80135ef0(g);
    } else {
        table_8016e7ac[data_8018f5a0->substep_index](g);
        if (g->field_4f != 0) {
            return;
        }
        x = 0;
        if (g->countdown_timer != 0) {
            g->countdown_timer_b--;
            x = 1;
            if (g->countdown_timer_b == 0) {
                x = 0;
                g->countdown_timer--;
                g->countdown_timer_b = g->reload_value;
            }
        }
        g->frame_counter++;
        g->field_a8 = x;
        if ((g->field_84 & 0x80) == 0) {
            func_801385a0(g);
            func_80128978();
            g->field_65 = player_left.field_165 | player_right.field_165;
            update_units();
            separate_overlapping_players();
            func_8012510c();
            func_8013c6ac();
            func_80138164();
            func_801510bc();
            if (data_80190568 != 0) {
                func_80135ef0(g);
            }
            if (g->state_param != 0) {
                g->state_param--;
            }
            func_80138ec4();
            func_80132e84();
        } else if (g->field_84 == 0x81) {
            g->field_84 = 0;
        }
    }
    func_8011a784();
}
