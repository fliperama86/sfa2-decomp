/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void finish_sequence_choose_next_with_sound(Object *object) {
    Config *config;
    if ((s16)object->step_flags < 0) {
        config = game_state.config;
        if (config->table_index != 0) {
            set_state_2_7_and_sequence_d(object);
        } else {
            if (*(Object **)&config->object_ptr != 0) {
                ref_first.p = *(Object **)&config->object_ptr;
                if (ref_first.p->mode_flag_2 != 0) {
                    play_sound_at(0, 0, 0x209);
                } else {
                    play_sound_at(0, 0, 0x208);
                }
            }
            if ((u8)is_config_object_health_full(object)) {
                init_move_left_from_right_sequence_c(object);
            } else {
                advance_state_0_and_clear_config_mode(object);
            }
        }
    }
}
