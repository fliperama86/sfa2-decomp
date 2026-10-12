/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d56c_slot12;
extern Object *data_8002d568_slot12;
extern u16 data_800175cc_slot12[];

void play_sounds_at_countdown_timer_values_slot12(void) {
    Object *o = data_8002d56c_slot12;
    int k = (s16)o->countdown_timer;
    u16 t;

    if (k == 0x5f) {
        t = o->field_b2;
        if ((unsigned)(t - 1) < 5) {
            play_sound_at(0, 0, data_800175cc_slot12[(s16)t]);
        } else {
            play_side_sound_at(data_8002d568_slot12, data_8002d568_slot12->side, data_800175cc_slot12[(s16)t]);
        }
    } else if (k == 0x5e) {
        play_side_sound_at(data_8002d568_slot12, data_8002d568_slot12->side, 7);
    }
}
