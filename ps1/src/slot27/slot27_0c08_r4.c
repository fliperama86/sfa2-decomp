/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

void func_80010fa8_slot27(void) {
    int i;
    int one;
    i = 0;
    one = 1;
    for (; i < 2; i++) {
        Object *p = &player_left + i;
        Object *o;
        if ((game_state.field_07 >> p->side) & 1) {
            p->field_d4 = 0;
        }
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_00 = one;
            o->field_02 = 0xac;
            o->other = p;
        }
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_00 = one;
            o->field_02 = 0xc;
            o->other = p;
        }
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_02 = 0x10;
            o->field_00 = one;
            o->field_03 = 3;
            o->other = p;
        }
        o = (Object *)func_8011f1e0();
        if (o != 0) {
            o->field_00 = one;
            o->field_02 = 0xd;
            o->other = p;
        }
    }
}
