/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801beff4_slot04_01[];
extern u8 data_801beffc_slot04_01[];

void set_pos_y_from_table_and_run_step_events_slot04_01(Object *o) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    int y;
    int t;
    int i;
    int v;
    int w;

    y = (u16)o->ground_y;
    o->pos_y = y;
    o->pos_y = y = y - *(u16 *)((u8 *)data_801beff4_slot04_01 + (o->step_flags & 0xe));
    t = (s16)o->step_flags;
    y = t;
    i = 0;
    t = t & 0xff00;
    if (t == 0x200) {
        o->step_flags = y & 0xff;
        o->field_165 = 0;
        if (o->field_4b == 0) {
            o->other->field_6b = 0xa;
            i = (o->direction_index >> 1) + 1;
        }
        o->other_lock = data_801beffc_slot04_01[i];
        play_side_sound_at(o, o->side, 5);
    }
    w = (s16)o->step_flags;
    v = w;
    w = w & 0xff00;
    if (w == 0x100) {
        o->step_flags = v & 0xff;
        play_sound_at(o, o->side, 0x31c);
        spawn_children_per_direction_index(o, -0x18, 0x1c);
    }
}
