/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c3c60_slot04_08[];

void func_801b1f8c_slot04_08(Object *obj) {
    u16 t;
    Object *p;
    t = obj->step_flags;
    if ((u8)t != 0) {
        if ((t & 0x80) == 0) {
            obj->step_flags = t & 0xff00;
            game_state.state_param = 0x18;
            func_80146960(obj);
        }
        obj->state_3 = obj->state_3 + 1;
        func_80140cd8(obj, *(s16 *)(data_801c3c60_slot04_08 + (obj->direction_index & 0xfe)), 0);
        p = obj->other;
        obj->countdown = 0x18;
        if ((s16)p->health < 0) {
            play_sound_at(obj, obj->side, 0x319);
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x18;
                obj->field_255 = 6;
                game_state.spawn_kind = 0;
                func_80147000(obj);
            }
        } else {
            play_sound_at(obj, obj->side, 0x319);
        }
    }
    update_sequence(obj);
}

void func_801b2088_slot04_08(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3 = obj->state_3 + 1;
        obj->facing_side = obj->facing_side ^ 1;
        func_801307e0(obj, 0x3c);
    } else {
        update_sequence(obj);
    }
}
