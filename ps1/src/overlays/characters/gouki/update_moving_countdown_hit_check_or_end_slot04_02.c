/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_moving_countdown_hit_check_or_end_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p = o->other;

    o->countdown = (s16)o->countdown - 1;
    if ((o->countdown & 0xff) != 0) {
        *(s32 *)&o->pos_x_frac += o->vel_x;
        if (((o->bound_push_flag >> o->variant_index) & 1) == 0) {
            if ((u8)try_force_other_state_if_box_in_range(o, -0x10, 0x10) == 0) {
                update_sequence(o);
            } else {
                o->state_3++;
                o->countdown = (o->countdown & 0xff00) | 0x10;
                play_sound_at(p, p->side, 0x31a);
                set_sequence_by_side_and_mode(o, 0x3b);
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

void count_down_then_advance_state_3_slot04_02(Object *o) {
    o->countdown = (s16)o->countdown - 1;
    if ((o->countdown & 0xff) == 0) {
        o->state_3++;
        o->countdown = o->countdown | 0x3b;
        fill_palette_banks_white();
        update_sequence(o);
    }
}
