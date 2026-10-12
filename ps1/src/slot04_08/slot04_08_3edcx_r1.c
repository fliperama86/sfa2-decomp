/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3edc_slot04_08(Object *obj, Object *unused) {
    int a;
    Object **q;
    Object **r = &ref_other.p;
    u16 t;
    *r = obj->other;
    t = obj->step_flags;
    if ((u8)t != 0) {
        obj->step_flags = t & 0xff00;
        func_80146478(obj, 1, -0x1d, 0x52);
        a = 1;
        if (((Slot04aObj *)obj)->field_1c7 == 0) {
            ((Slot04aObj *)obj)->field_1c7 = 0xff;
            a = 0xa;
        }
        if ((u8)func_80140cd8(obj, a, 0) != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, 1);
            play_sound_at(game_state.other_object, ref_other.p->side, 0x336);
            goto tail;
        }
        obj->field_46++;
        play_side_sound_at(obj, obj->side, 0xc);
        q = &game_state.other_object;
        play_sound_at(*q, ref_other.p->side, 0x304);
        func_80140fe0(obj);
        if ((s16)obj->field_46 != 0) {
            *q = obj->other;
            if (game_state.other_object->field_15b == 0 || (u8)func_801410c8(obj) != 0) {
                goto act;
            }
        }
        update_sequence(obj);
        return;
    }
    func_80140fe0(obj);
    if ((s16)obj->field_46 != 0) {
        *r = obj->other;
        if (ref_other.p->field_15b == 0 || (u8)func_801410c8(obj) != 0) {
act:
            func_80140770(obj, 0, 5, 0, 0, 0, 1);
            play_side_sound_at(obj, obj->side, 0xd);
tail:
            obj->state_3++;
            func_801307e0(obj, 0x1b);
            return;
        }
    }
    update_sequence(obj);
}
