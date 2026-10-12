/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3e98_slot04_08;

void move_x_spawn_children_until_bound_reached_slot04_08(Object *obj) {
    u16 t;
    s16 a;
    Object *p;
    Object *q;
    unsigned lim;
    int v;
    int n;
    u16 x;

    t = (obj->countdown + 1) & 7;
    obj->countdown = t;
    if (t == 0) {
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 8;
            p->table_index = 1;
            p->parent = obj;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y - 0x6c;
            p->spawn_x = obj->spawn_x;
            p->spawn_y = obj->spawn_y;
            p->image_row = obj->image_row;
            p->mode_id = 0x20;
            p->sound_side = obj->sound_side;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
        }
    }
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    x = *(u16 *)&obj->pos_x;
    v = x - data_801c3e98_slot04_08;
    a = v;
    if (obj->facing_side != 0) {
        a = (u16)-v;
    }
    if ((s16)a >= 0) {
        a = 2;
        if (obj->vel_x < 0) {
            a = 1;
        }
        if (obj->bound_push_flag != (s16)a) {
            q = obj->other;
            a = -0x20;
            if (obj->facing_side != 0) {
                a = 0x20;
            }
            a = (a + x - *(u16 *)&q->pos_x) + 0x40;
            lim = 0x80;
            if (lim < (u16)a) {
                update_sequence(obj);
                return;
            }
        }
    }
    obj->state_3++;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x80000;
    } else {
        obj->vel_x = -0x80000;
    }
    n = 0x20;
    ((Slot04aObj *)obj)->field_1c8 = 8;
    if (obj->field_49 != 0) {
        n = 0x68;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + n);
}
