/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_pair_selection_or_reset_by_mode_flag_a(Entity *entity) {
    Object *s1;
    Object *s2;
    if (entity->mode_flag_a == 0) {
        append_object_bytes_to_ring((GameState *)entity);
        ((HudBig *)screen_state)->field_4c++;
        entity->active_flag_2 = 1;
        player_left.field_a5 = 0;
        player_right.field_a5 = 0;
        s1 = entity->field_78;
        s2 = entity->field_7c;
        update_best_scores_for_pair(entity, s1, s2);
        clear_flag_if_mode_3_or_other_flag_or_zero(entity, s1);
        entity->kind = s1->kind;
        select_next_kind_for_object(entity, s1, s2);
        if (entity->field_90 != 0) {
            init_entity_from_other_object(entity, s1);
        } else {
            if (s2->side == 0) {
                entity->field_1b &= 0xfe;
            } else if (s2->side == 1) {
                entity->field_1b &= 0xfd;
            }
            if (s1->mode_flag_2 != 0) {
                set_object_flags_and_hud_step_3(s2, entity);
            } else {
                s1->field_a5 = 2;
                s2->mode_flag_2 = 1;
            }
        }
    } else {
        ((HudBig *)screen_state)->field_4a = 1;
        ((HudBig *)screen_state)->field_4c = 0;
        ((HudBig *)screen_state)->step_count = 0;
        ((HudBig *)screen_state)->state_index = 0;
        entity->field_1a = 1;
        player_left.field_a5 = 0;
        player_right.field_a5 = 0;
        if (player_left.kind == 0x13) {
            player_left.kind = 0x11;
        }
        if (player_right.kind == 0x13) {
            player_right.kind = 0x11;
        }
        entity->field_1b = 0;
        entity->field_07 = 3;
        entity->field_17 = 3;
        entity->countdown = 0;
        kind_handlers_enabled = 0;
        init_block172_pool();
        entity->active_flag_2 = 1;
    }
}
