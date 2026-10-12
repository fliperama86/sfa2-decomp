/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801443b0(Object *object) {
    Config *config;
    if ((s16)object->step_flags < 0) {
        config = game_state.config;
        if (config->table_index != 0) {
            func_8014448c(object);
        } else {
            if (*(Object **)&config->object_ptr != 0) {
                ref_first.p = *(Object **)&config->object_ptr;
                if (ref_first.p->mode_flag_2 != 0) {
                    play_sound_at(0, 0, 0x209);
                } else {
                    play_sound_at(0, 0, 0x208);
                }
            }
            if ((u8)func_801441c8(object)) {
                func_80144090(object);
            } else {
                func_80144e90(object);
            }
        }
    }
}
