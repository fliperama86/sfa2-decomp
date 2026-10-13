/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: same code, but the original keeps the second 16-bit read in $a1 and the kind in $v1 (registers swapped). */
void init_entity_from_other_object(ObjectView *entity, Object *other) {
    HudBig *hud;
    entity->kind_table_value = table_8016f4f0[other->kind];
    entity->field_62 = other->field_d4;
    entity->other_side = other->side;
    hud = (HudBig *)screen_state;
    hud->field_4a = 5;
    hud->field_4c = 0;
    entity->field_c6 = 0;
    hud->step_count = 0;
    entity->field_c8 = 0;
    hud->state_index = 0;
    entity->field_ca = 0;
    hud->substate_index = 0;
    entity->field_cc = 0;
    entity->field_46 = 1;
    entity->field_09 = 1;
    entity->mode = 0;
    entity->field_27 = 0;
    entity->field_09 = 1;
    player_left.field_a5 = 0;
    player_right.field_a5 = 0;
    entity->field_8e = 0;
}

void clear_hud_and_entity_fields_then_switch_entry(GameState *state) {
    ObjectView *entity = (ObjectView *)state;
    HudBig *hud = (HudBig *)screen_state;
    hud->field_48 = 0;
    entity->field_c2 = 0;
    hud->field_4a = 0;
    entity->field_c4 = 0;
    hud->field_4c = 0;
    entity->field_c6 = 0;
    hud->step_count = 0;
    entity->field_c8 = 0;
    hud->state_index = 0;
    entity->field_ca = 0;
    hud->substate_index = 0;
    entity->field_cc = 0;
    entity->field_ee = 0;
    entity->field_f0 = 0;
    entity->field_20 = 0;
    switch_to_next_table_entry_and_wait();
}

void fill_zero_entries_with_8_up_to_count(ObjectView *entity, s16 count) {
    int i;
    entity->entry_count = count;
    if (count >= 0) {
        for (i = 0; i < count; i++) {
            if (entity->slot_values[i] == 0) {
                entity->slot_values[i] = 8;
                data_80190474 = 2;
            }
        }
    }
}

void set_other_marker_and_play_side_sound_3(ObjectView *entity, ObjectView *other) {
    other->field_116 = 0xff;
    play_side_sound_at((Object *)entity, entity->side, 3);
}

void clear_flag_if_mode_3_or_other_flag_or_zero(ObjectView *entity, Object *other) {
    if (entity->mode == 3 || other->mode_flag_2 != 0 || entity->field_60 == 0) {
        entity->field_77 = 0;
    }
}

void set_random_flag_by_chance_when_table_index_6(ObjectView *entity) {
    entity->random_flag = 0;
    if (entity->table_index == 6) {
        if ((get_random_byte() & 0x3f) == 0) {
            entity->random_flag = 1;
        }
    }
}
