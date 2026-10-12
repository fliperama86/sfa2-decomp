/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3cc4_slot04_08[];

void func_801b25a8_slot04_08(Object *obj);

void func_801b2720_slot04_08(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        func_801b25a8_slot04_08(obj);
    }
}

void func_801b275c_slot04_08(Object *obj) {
    int a = 0x64;

    obj->state_3++;
    obj->facing_side ^= 1;
    if (obj->direction_index != 0) {
        a = 0x78;
    }
    func_801307e0(obj, a);
}

void func_801b27a4_slot04_08(Object *obj) {

    u16 t = obj->step_flags;

    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        game_state.state_param = 0x18;
        func_80146960(obj);
        if ((u8)func_80140cd8(obj, (s16)data_801c3cc4_slot04_08[((Slot04aObj *)obj)->step_index + (obj->direction_index << 1)], 0) != 0) {
            game_state.spawn_kind = 4;
            func_80147000(obj);
        }
        obj->countdown = 0x10;
        obj->state_3++;
        play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x319);
    }
    update_sequence(obj);
}
