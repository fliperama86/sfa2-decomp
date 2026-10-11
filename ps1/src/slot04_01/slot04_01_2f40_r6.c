/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3408_slot04_01(Object *obj) {
    u16 t = obj->step_flags;
    Object *p = obj->other;

    if ((u8)t != 0) {
        obj->step_flags = t & 0xff00;
        func_80146478(obj, 1, -0x3e, 0x39);
        play_side_sound_at(obj, ((Slot04aObj *)obj)->field_a6, 8);
        if ((u8)func_80140cd8(obj, 0, 0) != 0) {
            func_80140770(obj, 1, 5, 0, 0, 0, 0);
            obj->state_3++;
            play_sound_at(p, ((Slot04aObj *)p)->field_a6, 0x30b);
            play_side_sound_at(obj, ((Slot04aObj *)obj)->field_a6, 7);
            func_801307e0(obj, 0x2f);
            return;
        }
        obj->field_46++;
        play_sound_at(p, ((Slot04aObj *)p)->field_a6, 0x30b);
    }
    func_80140fe0(obj);
    if ((s16)obj->field_46 == 0 || (p->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
        update_sequence(obj);
        return;
    }
    func_80146478(obj, 1, -0x3e, 0x39);
    func_80140770(obj, 1, 5, 0, 0, 0, 1);
    play_sound_at(p, ((Slot04aObj *)p)->field_a6, 0x30d);
    obj->state_3++;
    play_side_sound_at(obj, ((Slot04aObj *)obj)->field_a6, 7);
    func_801307e0(obj, 0x2f);
}
