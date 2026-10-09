/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Block172 *func_8011f1e0(void);

void func_8012867c(void) {
    u16 *p = table_8016f658[game_state.field_40];
    Object *o;
    s16 h;
    while ((h = *p++) >= 0) {
        if (h != 0)
            break;
        o = (Object *)func_8011f1e0();
        if (o == 0)
            break;
        o->field_00++;
        o->pos_x = *p++;
        o->pos_y = *p++;
        o->field_09 = *p++;
        o->field_02 = *p++;
        o->field_03 = *p++;
        o->field_0b = *p++;
        o->field_0e = *p++;
        o->field_1e = *p++;
        o->field_1c = *p++;
        o->field_26 = *p++;
    }
}
