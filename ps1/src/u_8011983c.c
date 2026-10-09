/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011983c(void) {
    int z = 0;
    GameState *g = &game_state;
    Tune2a *t = &table_8016e664;
    HudState *h;
    func_80157d00(1);
    data_8018f5a0->field_48 = 0;
    for (;;) {
        h = data_8018f5a0;
        switch (h->field_48) {
        case 0:
            data_8019093c = z;
            data_80190940 = z;
            func_8011eb14();
            func_8011a744();
            func_801199ec();
            if (*(int *)0x1f800000 == 0) {
                func_801201e8();
            }
            data_8018f5a0->field_48 = data_8018f5a0->field_48 + 1;
            break;
        case 1:
            h->field_48 = h->field_48 + 1;
        case 2:
            g->field_0e = 1;
            g->field_0f = z;
            g->field_2b = z;
            g->field_ba = z;
            g->field_11 = t->field_21;
            g->field_10 = t->field_22;
            if (t->field_24 == 0) {
                g->field_13 = t->field_23;
            } else {
                g->field_13 = 0xff;
            }
            g->field_15 = t->field_25 + 1;
            g->field_12 = t->field_26;
            g->field_14 = t->field_28;
            g->field_2a = t->field_29;
            data_8018f5a0->field_48 = 0;
            data_8018f5a0->field_4a = 0;
            data_8018f5a0->field_4c = 0;
            data_8018f5a0->field_4e = 0;
            data_8018f5a0->field_50 = 0;
            func_80152ee8();
            break;
        }
        func_801192bc(1);
    }
}
