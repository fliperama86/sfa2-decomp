/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c6500_slot04_02[];
extern u8 data_801c6508_slot04_02[];

void func_801b4f0c_slot04_02(Object *obj) {
    Object *p;
    u16 t;

    p = obj->other;
    update_sequence(obj);
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3 = obj->state_3 + 1;
        obj->field_17b = 0;
        p->field_15b = 1;
        func_80140770(obj, 0, 0xf, *(s16 *)(data_801c6500_slot04_02 + (obj->field_12a & 0xfe)), 0, 0, 1);
        func_801307e0(obj, 0x51);
        if (((Slot04aObj *)p)->field_5c < 0) {
            if (obj->field_49 != 0) {
                obj->field_167 = 0x12;
                obj->field_255 = 6;
                game_state.field_6b = 0;
                func_80147000(obj);
            } else {
                obj->field_167 = 2;
            }
        }
    }
    t = obj->step_flags;
    if ((u8)t != 0) {
        obj->step_flags = t & 0xff00;
        func_80146478(obj, 2, -0x17, 0x26);
        play_sound_at(p, p->side, 0x306);
        if (game_state.field_5c == 0) {
            game_state.field_5c = 0xa;
            game_state.field_5d = 2;
            game_state.reload_value = 2;
        }
        game_state.field_63 = data_801c6508_slot04_02[obj->field_12a >> 1];
    }
}
