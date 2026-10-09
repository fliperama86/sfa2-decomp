/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012306c(GameState *g) {
    Object *o = g->field_78;
    if (o->field_cd == 0) {
        func_8012304c((Entity *)g);
        if ((u8)func_80125394() != 0 || g->field_108 > g->field_104) {
            data_8018f5a0->field_50++;
            g->field_ca = 0x5a;
            o->field_b8 = func_80155f98(o->field_b8, g->field_104 << 8);
            g->field_104 = 0;
            func_80156094();
            func_80120554(0, 0, 0x34d);
            func_80123504(g);
        } else {
            g->field_104 = func_80156018(g->field_104, g->field_108);
            o->field_b8 = func_80155f98(o->field_b8, g->field_108 << 8);
            func_80156094();
            if ((game_state.field_1d & 3) == 0) {
                func_80120554(0, 0, 0x34d);
            }
        }
    }
}
