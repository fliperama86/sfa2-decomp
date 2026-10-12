/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c74f4_slot04_11[];
extern u8 data_801c74f8_slot04_11[][4];

void spawn_children_per_direction_index(Object *object, int a, int b);
void update_dash_grab_or_end_slot04_11(Object *obj);

void update_sequence_with_step_flag_actions_slot04_11(Object *obj) {
    int i;
    u8 t;

    update_sequence(obj);
    if ((obj->step_flags & 1) == 0) {
        if (obj->field_165 != 0) {
            i = 6;
            if (obj->field_4b == 0) {
                ref_other.p = obj->other;
                ref_other.p->field_6b = 10;
                i = obj->direction_index;
            }
            t = data_801c74f8_slot04_11[i >> 1][0];
            obj->field_165 = 0;
            obj->other_lock = t;
        }
    }
    if (obj->step_flags & 2) {
        obj->step_flags &= 0xfffd;
        spawn_children_per_direction_index(obj, 0x28, 0x66);
        play_sound_at(obj, obj->side, 0x31c);
    }
    if (obj->step_flags & 4) {
        int d;
        u16 w;

        w = obj->step_flags & 0xfffb;
        obj->step_flags = w;
        d = (w >> 8) & 0x7f;
        if (obj->facing_side == 0) {
            d = -d;
        }
        obj->pos_x += d;
    }
    if (obj->step_flags & 8) {
        obj->state_3++;
        obj->vel_x = data_801c74f4_slot04_11[obj->direction_index];
        ((ObjectWords *)obj)->word_54 = data_801c74f4_slot04_11[obj->direction_index + 1] & 0xffff0000;
        *(u16 *)&((ObjectWords *)obj)->word_54 = obj->pos_x;
        update_dash_grab_or_end_slot04_11(obj);
    }
}
