/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801c3e98_slot04_08;

void push_other_x_if_hit_check_slot04_08(Object *obj);

void move_x_toward_target_then_set_sequence_slot04_08(Object *obj) {
    u16 t;
    u16 a;
    unsigned lim;
    Object *p;
    int v;
    u8 f;
    int n;
    u16 x;

    t = (obj->countdown + 1) & 7;
    obj->countdown = t;
    if (t == 0) {
        push_other_x_if_hit_check_slot04_08(obj);
    }
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    x = *(u16 *)&obj->pos_x;
    f = obj->facing_side;
    v = x - data_801c3e98_slot04_08;
    a = v;
    if (f != 0) {
        a = -v;
    }
    if ((s16)a >= 0) {
        a = 2;
        if (obj->vel_x < 0) {
            a = 1;
        }
        if (obj->bound_push_flag != (s16)a) {
            p = obj->other;
            v = -0x20;
            a = v;
            if (f != 0) {
                a = 0x20;
            }
            a = (a + x - *(u16 *)&p->pos_x) + 0x40;
            lim = 0x80;
            if (lim < (u16)a) {
                update_sequence(obj);
            } else {
                goto tail;
            }
        } else {
            goto tail;
        }
    } else {
tail:
        n = 0x29;
        ((Slot04aObj *)obj)->field_1c8 = 8;
        obj->state_3++;
        if (obj->field_49 != 0) {
            n = 0x71;
        }
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + n);
    }
}
