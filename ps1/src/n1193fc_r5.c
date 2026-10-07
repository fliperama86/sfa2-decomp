/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011f09c(void) {
    u8 *p = &data_801ae02c;
    u32 i;
    for (i = 0; i < 0xac; i++) {
        *p++ = 0;
    }
    (&data_801ae02c)[8] = 0x18;
    game_state.field_06 = 0;
    game_state.field_04 = 0;
    game_state.field_05 = 0;
}

/* The parameter is unused here. It is declared because calls found so far, in character files of slot 0x4, set the first argument register in the delay slot of the call; that is compatible with a parameter and does not prove one. */
Object *func_8011f0e8(Object *unused) {
    Object *o;
    Object *result;
    int n = data_801a6960;
    if (n != 0) {
        o = data_801a89b0[n];
        data_801a6960 = n - 1;
        o->field_a1 = 0;
        func_80119b34((AnimObj *)o);
        result = o;
    } else {
        result = 0;
    }
    return result;
}
