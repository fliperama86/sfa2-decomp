/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_blocks(void) {
    Block172 *b = (Block172 *)data_801a89f4;

    game_state.field_24 = 0x28;
    do {
        if (b->active_flag != 0 && kind_handlers_enabled != 0) {
            if (b->pool_kind == 0x20) {
                if (b->handler_table_select == 0) {
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
