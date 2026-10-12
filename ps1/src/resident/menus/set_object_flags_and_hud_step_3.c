/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void set_object_flags_and_hud_step_3(Object *a, Entity *b) {
    a->mode_flag_2 = 1;
    ((HudBig *)data_8018f5a0)->field_4c = 3;
    b->ready_flag = 1;
    b->active_flag_2 = 1;
    b->field_1b = 0;
    player_left.field_a5 = 0;
    player_right.field_a5 = 0;
}

void advance_hud_step_and_reset_entity_fields(Entity *entity) {
    update_best_scores_for_pair(entity, &player_left, &player_left + 1);
    ((HudBig *)data_8018f5a0)->field_4a++;
    ((HudBig *)data_8018f5a0)->field_4c = 0;
    entity->field_c6 = 0;
    ((HudBig *)data_8018f5a0)->step_count = 0;
    entity->field_c8 = 0;
    ((HudBig *)data_8018f5a0)->state_index = 0;
    entity->countdown_timer = 0;
    ((HudBig *)data_8018f5a0)->substate_index = 0;
    entity->field_cc = 0;
    entity->field_77 = 0;
    entity->table_index = 0;
    entity->ready_flag = 1;
    entity->active_flag_2 = 1;
    entity->field_1b = 0;
    player_left.field_01 = 0;
    player_right.field_01 = 0;
    player_left.field_a5 = 0;
    player_right.field_a5 = 0;
    player_left.mode_flag_2 = 1;
    player_right.mode_flag_2 = 1;
    data_80190568 = 0;
}
