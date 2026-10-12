/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: same code, but the original keeps the second 16-bit read in $a1 and the kind in $v1 (registers swapped). */
void func_801239cc(ObjectView *entity, Object *other) {
    HudBig *hud;
    entity->kind_table_value = table_8016f4f0[other->kind];
    entity->field_62 = other->field_d4;
    entity->other_side = other->side;
    hud = (HudBig *)data_8018f5a0;
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

void func_80123a64(GameState *state) {
    ObjectView *entity = (ObjectView *)state;
    HudBig *hud = (HudBig *)data_8018f5a0;
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
    func_80152ee8();
}

void func_80123ac4(ObjectView *entity, s16 count) {
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

void func_80123b20(ObjectView *entity, ObjectView *other) {
    other->field_116 = 0xff;
    play_side_sound_at((Object *)entity, entity->side, 3);
}

void func_80123b4c(ObjectView *entity, Object *other) {
    if (entity->mode == 3 || other->mode_flag_2 != 0 || entity->field_60 == 0) {
        entity->field_77 = 0;
    }
}

void func_80123b88(ObjectView *entity) {
    entity->random_flag = 0;
    if (entity->table_index == 6) {
        if ((get_random_byte() & 0x3f) == 0) {
            entity->random_flag = 1;
        }
    }
}
