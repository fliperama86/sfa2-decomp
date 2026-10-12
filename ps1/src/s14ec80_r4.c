/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014f76c(Ctl *p) {
    int i;
    for (i = 0; i < 4; i++) {
        if (p->slots[i]) {
            close_vab_and_sep(i);
        }
    }
    p->state_index = 6;
    p->field_15 = 0;
    p->field_16 = 0;
    p->field_17 = 0;
    p->field_1c = 0;
    p->field_1f = 0;
    set_sprite_fields_from_table(p, p->handler_arg);
    seek_cd_position_with_retries(p);
}
