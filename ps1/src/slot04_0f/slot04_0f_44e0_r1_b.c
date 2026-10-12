/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c54dc_slot04_0f[];
extern ObjectFn data_801c5e38_slot04_0f[];

void noop_empty_function_slot04_0f(void) {
}

void play_side_sound_from_step_flag_byte_slot04_0f(Object *obj) {
    u16 t = obj->step_flags;
    u8 k = t;

    if (k != 0) {
        obj->step_flags = t & 0x80ff;
        play_side_sound_at(obj, obj->side, data_801c54dc_slot04_0f[k]);
    }
}

void call_handler_by_state_0_slot04_0f(Object *obj) {
    data_801c5e38_slot04_0f[obj->state_0](obj);
}
