/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c63d4_slot04_02[];

void update_state_3_spawn_child_on_step_flag_slot04_02(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    s16 t;
    int i;

    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x80) {
        obj->field_165 = 0;
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_4b != 0) {
            obj->other_lock = 0;
        } else {
            obj->other_lock = 1;
        }
    } else if ((t & 0xff) != 0) {
        if (obj->field_165 == 0) {
            if (obj->field_4b != 0) {
                obj->field_165 = 1;
            } else {
                obj->field_165 = 0xff;
            }
        }
        play_sound_at(obj, obj->side, 0x31c);
        i = (((u8)obj->step_flags - 1) << 2) & 0x1fc;
        obj->step_flags = obj->step_flags & 0xff00;
        spawn_child_object_with_offsets(obj, *(s16 *)((u8 *)data_801c63d4_slot04_02 + i), *(s16 *)((u8 *)data_801c63d4_slot04_02 + i + 2));
    }
}
