/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


#define UNIT_INDEX (*(u8 *)&game_state.field_354)

void update_enabled_units_by_handler_index(void) {
    u8 k;
    for (UNIT_INDEX = 0; UNIT_INDEX < 16; UNIT_INDEX++) {
        k = UNIT_INDEX;
        if (units_2c20[k].active != 0 && units_2c20[k].handler_enable != 0) {
            if (units_2c20[k].handler_index == 0x17) {
                build_block_primitives(&units_2c20[k]);
            } else {
                reload_stream_cells_then_build_sprite_quads((Slab172 *)&units_2c20[k]);
            }
        }
    }
}
