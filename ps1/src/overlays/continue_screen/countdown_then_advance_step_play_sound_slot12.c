/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_8002d56c_slot12;
extern Object *data_8002d568_slot12;

void countdown_then_advance_step_play_sound_slot12(void) {
    u16 *t = &game_state.countdown_2;
    int v = *t - 1;

    *t = v;
    if ((s16)v < 0) {
        data_8018f5a0->step_index++;
        game_state.status_flags = 0xff;
        *t = 0x1f;
        if (game_state.field_44 & 0x80) {
            play_side_sound_at(data_8002d56c_slot12, game_state.sound_side ^ 1, 6);
        } else {
            play_side_sound_at(data_8002d568_slot12, data_8002d568_slot12->side, 6);
        }
    }
}

void countdown_then_advance_step_clear_status_slot12(void) {
    u16 *t = &game_state.countdown_2;
    int v = *t - 1;

    *t = v;
    if ((s16)v < 0) {
        data_8018f5a0->step_index++;
        game_state.status_flags = 0;
        *t = 0x78;
    }
}
