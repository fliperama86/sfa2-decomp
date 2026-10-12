/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c7cb8_slot04_09[];
void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b2548_slot04_09(Object *obj) {
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (obj->vel_y >= 0 && (s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->facing_side ^= 1;
        func_801307e0(obj, 0x49);
    }
}

void func_801b25cc_slot04_09(Object *obj) {
    Object *o;

    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        o = obj->other;
        o->field_15b = 1;
        game_state.state_param = 0x18;
        func_80140770(obj, 5, 10, *(s16 *)((u8 *)data_801c7cb8_slot04_09 + (obj->direction_index & 0xfe)), 0, 1, 0);
        o = obj->other;
        if ((s16)o->health < 0) {
            obj->field_167 = 0x19;
            obj->field_255 = 6;
            game_state.spawn_kind = 0;
            func_80147000(obj);
        }
    }
}
