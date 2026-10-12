/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80138164(void) {
    Block172 *b = (Block172 *)data_801a89f4;

    game_state.field_24 = 0x28;
    do {
        if (b->active_flag != 0 && data_80190568 != 0) {
            if (b->field_08 == 0x20) {
                if (b->field_66 == 0) {
                    table_80172980[b->handler_index](b);
                } else {
                    table_801729e4[b->handler_index](b);
                }
            } else {
                table_801726bc[b->handler_index](b);
            }
        }
        b++;
    } while (--game_state.field_24 != 0);
}
