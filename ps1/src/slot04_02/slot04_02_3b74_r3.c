/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3ef8_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p = o->other;

    o->countdown = (s16)o->countdown - 1;
    if ((o->countdown & 0xff) != 0) {
        *(s32 *)&o->pos_x_frac += o->vel_x;
        if (((o->bound_push_flag >> o->variant_index) & 1) == 0) {
            if ((u8)func_8013f8c4(o, -0x10, 0x10) == 0) {
                update_sequence(o);
            } else {
                o->state_3++;
                o->countdown = (o->countdown & 0xff00) | 0x10;
                play_sound_at(p, p->side, 0x31a);
                func_801307e0(o, 0x3b);
            }
        } else {
            o->state_3 = 8;
            o->countdown = obj->flag_bits | 0x800;
            set_sequence_by_side(o, 0);
        }
    } else {
        o->state_3 = 8;
        o->countdown = obj->flag_bits | 0x800;
        set_sequence_by_side(o, 0);
    }
}

void func_801b3fec_slot04_02(Object *o) {
    o->countdown = (s16)o->countdown - 1;
    if ((o->countdown & 0xff) == 0) {
        o->state_3++;
        o->countdown = o->countdown | 0x3b;
        func_80126130();
        update_sequence(o);
    }
}
