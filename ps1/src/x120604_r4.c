/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;


void func_80121f74(GameState *g) {
    int x;
    if (((g->field_30 == 0 && g->field_226 == 0) ? g->field_4c : (g->field_4c || g->field_226)) == 0) {
        func_80123e34();
    }
    if (g->field_31 != 0) {
        if (g->field_74 == 0) {
            func_80154584();
        }
    } else if (g->field_30 != 0 && data_80197f1c != 0) {
        if (g->field_74 == 0) {
            func_801241d8();
        }
        func_80138164();
        func_801510bc();
        func_80135ef0(g);
    } else {
        table_8016e7ac[data_8018f5a0->field_4e](g);
        if (g->field_4f != 0) {
            return;
        }
        x = 0;
        if (g->field_5c != 0) {
            g->field_5d--;
            x = 1;
            if (g->field_5d == 0) {
                x = 0;
                g->field_5c--;
                g->field_5d = g->field_5e;
            }
        }
        g->field_1d++;
        g->field_a8 = x;
        if ((g->field_84 & 0x80) == 0) {
            func_801385a0(g);
            func_80128978();
            g->field_65 = player_left.field_165 | player_right.field_165;
            func_80138290();
            func_8013902c();
            func_8012510c();
            func_8013c6ac();
            func_80138164();
            func_801510bc();
            if (data_80190568 != 0) {
                func_80135ef0(g);
            }
            if (g->field_63 != 0) {
                g->field_63--;
            }
            func_80138ec4();
            func_80132e84();
        } else if (g->field_84 == 0x81) {
            g->field_84 = 0;
        }
    }
    func_8011a784();
}
