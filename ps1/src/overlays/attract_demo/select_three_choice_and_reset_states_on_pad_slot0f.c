/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_800e8790_slot0f[];
extern void (*data_800e8794_slot0f[])(Object *);

void select_three_choice_and_reset_states_on_pad_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u16 b = o->pos_x_frac;
    if (b & 0x5000) {
        if (b & 0x1000) {
            obj->field_0a -= 1;
            if (obj->field_0a < 0) {
                o->field_0a = 2;
            }
        } else {
            o->field_0a += 1;
            if ((s8)o->field_0a >= 3) {
                o->field_0a = 0;
            }
        }
        play_sound_at(0, 0, 0x204);
        b = o->pos_x_frac;
    }
    if (b & 0x860) {
        if (b & 0x820) {
            o->active_count = data_800e8790_slot0f[(s8)o->field_0a];
            play_sound_at(0, 0, 0x205);
        } else {
            o->active_count = 5;
            play_sound_at(0, 0, 0x202);
        }
        o->table_index = 0;
        o->field_02 = 0;
        o->field_01 = 0;
        o->state_3 = 0;
        o->state_2 = 0;
        o->state_1 = 0;
        o->state_0 = 0;
    }
}

void func_800e182c_slot0f(Object *o) {
    data_800e8794_slot0f[(s8)o->field_01](o);
}
