/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void advance_state_index_and_set_menu_command_by_kind(GameState *state) {
    Entity *entity = (Entity *)state;
    ((HudBig *)screen_state)->state_index++;
    entity->field_6c = 1;
    if (entity->pass_count == 0) {
        set_menu_command_by_code(1, table_8016e6f8[game_state.kind_index]);
    }
}
