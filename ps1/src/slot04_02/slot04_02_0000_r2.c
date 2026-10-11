/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b0118_slot04_02(Object *obj) {
    s16 t = obj->step_flags;

    if (t < 0) {
        func_801b631c_slot04_02(obj, 1, 0, 0, 0);
        func_80130678(obj, 0);
    } else {
        if ((t & 0xff) != 0) {
            obj->step_flags = t & 0xff;
            game_state.field_63 = 0x18;
            play_side_sound_at(obj, obj->side, 0xe);
        }
        update_sequence(obj);
    }
}
