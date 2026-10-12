/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 tick_countdown_spawn_random_effect_slot04_02(Object *obj);
void hit_other_and_spawn_if_health_negative_slot04_02(Object *obj);

void update_countdown_effect_advance_state_3_slot04_02(Object *obj) {
    Object *p = obj->other;

    fill_palette_banks_white();
    obj->countdown = (s16)obj->countdown - 1;
    if ((obj->countdown & 0xff) == 0) {
        obj->state_3++;
        play_sound_at(p, p->side, 0x306);
        call_side_func_then_queue_job(obj);
        restore_palette_banks();
        hit_other_and_spawn_if_health_negative_slot04_02(obj);
    } else if (tick_countdown_spawn_random_effect_slot04_02(obj) != 0) {
        if (obj->mode_flag_2 == 0) {
            obj->sound_index = 0;
            obj->field_bf = 0xff;
            add_table_bcd_score_to_player(4, (s8)obj->side);
            add_table_bcd_to_player_total(4, obj->side);
        }
        p->round_count++;
    }
}

void count_down_high_byte_then_reset_state_slot04_02(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) != 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
