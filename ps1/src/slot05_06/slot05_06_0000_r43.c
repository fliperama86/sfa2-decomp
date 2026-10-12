/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd484_slot05_06[];

void func_801cba68_slot05_06(Object *obj) {
    Object *other;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        other = obj->other;
        other->field_15b = 1;
        func_80140770(obj, 0, 0xc, 3, 0, 0, 0);
        if ((s16)other->health < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x16;
                obj->field_255 = 6;
                game_state.spawn_kind = 0;
                func_80147000(obj);
            }
        }
        other->step_applied_flag = 0;
        obj->active_flag_4 = 0;
        func_801312b8(obj);
    }
}

void func_801cbb2c_slot05_06(Object *obj) {
    data_801dd484_slot05_06[obj->state_3](obj);
}
