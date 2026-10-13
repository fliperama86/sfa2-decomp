/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void advance_pass_index_then_next_step(Object *object) {
    Entity *entity = (Entity *)object;
    int v;
    switch (screen_state->pass_index) {
    case 0:
        v = screen_state->sub_timer - 1;
        screen_state->sub_timer = v;
        if ((s16)v < 0) {
            screen_state->sub_timer = 4;
            screen_state->pass_index++;
            init_sprite_tables();
        }
        break;
    case 1:
        v = screen_state->sub_timer - 1;
        screen_state->sub_timer = v;
        if ((s16)v < 0) {
            screen_state->sub_timer = 0;
            screen_state->pass_index++;
        }
        break;
    case 2:
        screen_state->step_index++;
        screen_state->substep_index = 0;
        screen_state->step_index_2 = 0;
        screen_state->pass_index = 0;
        entity->field_ab = 0;
        if (entity->table_index != 0) {
            screen_state->step_index = 5;
        }
        entity->active_flag = 1;
        init_hud_primitives();
        if (entity->mode_flag_b != 0) {
            data_80197f1c = 0xff;
        }
        break;
    }
}
