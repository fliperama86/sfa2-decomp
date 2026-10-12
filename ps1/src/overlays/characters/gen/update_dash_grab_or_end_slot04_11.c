/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801c7514_slot04_11[];

int check_range_set_other_state_if_boxed(Object *object, s16 a, s16 b, s16 c, u16 d);
void update_dash_grab_or_end_slot04_11(Object *obj);

void update_dash_grab_or_end_slot04_11(Object *obj) {
    int a;
    int b;
    u8 k;
    int lo;
    int hi;
    s16 ox;
    int n;

    a = obj->vel_x;
    if (obj->facing_side == 0) {
        a = -a;
    }
    *(s32 *)&obj->pos_x_frac += a;
    if (obj->active_flag != 0) {
        ref_other.p = obj->other;
        if (ref_other.p->field_163 == 0 && *(u16 *)&ref_other.p->state_0 == 0x101
            && (u8)check_range_set_other_state_if_boxed(obj, 0, 0x40, 8, 8)) {
            obj->vel_x = 0xc0000;
            obj->state_3++;
            obj->accel_x = -0x8000;
            ref_other.p->field_261 = 1;
            ((Slot04bObj *)obj)->saved_pos_x = obj->pos_x;
            ref_other.p->pos_y = obj->pos_y;
            ref_other.p->active_state = 0;
            if ((s16)ref_other.p->health < 0) {
                obj->step_limit = 0xff;
            }
            n = obj->direction_index;
            n += 0x54;
            set_sequence_by_side_and_mode(obj, n);
            return;
        }
    }
    a = obj->pos_x;
    a -= *(s16 *)&obj->accel_x;
    b = *(s16 *)((u8 *)&obj->accel_x + 2);
    a += b;
    b <<= 1;
    if (!((u32)b < (u32)a)) {
        a = data_801c7514_slot04_11[obj->facing_side];
        if (!(a & obj->bound_push_flag)) {
            update_sequence(obj);
            return;
        }
    }
    b = -1;
    obj->state_3 += 2;
    ref_other.p = obj->other;
    lo = obj->pos_x;
    ox = ref_other.p->pos_x;
    hi = ox;
    k = obj->facing_side;
    if (k == 0) {
        int t = lo;
        lo = hi;
        hi = t;
        b = 1;
    }
    if (lo >= hi && (data_801c7514_slot04_11[k] & ref_other.p->bound_push_flag)) {
        ref_other.p->pos_x = ox + b;
        ref_other.p->bound_push_flag = 0;
    }
    obj->vel_x = 0x80000;
    obj->accel_x = 0xffff0000;
    set_sequence_by_side_and_mode(obj, 0x59);
}
