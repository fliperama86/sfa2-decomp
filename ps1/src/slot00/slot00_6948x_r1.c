/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800797dc_slot00[];
void select_box_tables_slot00_slot00(Object *obj);
void set_position_from_parent_offset_slot00(Object *obj);
void spawn_child_object_at_table_offset_slot00(Object *obj);

void func_80076948_slot00(Object *o) {
    unsigned i;
    int lim;
    set_position_from_parent_offset_slot00(o);
    i = o->active_flag_2 != 0;
    i = (-i & 6) + o->vel_table_index;
    i >>= 1;
    lim = data_800797dc_slot00[i];
    o->countdown++;
    if ((s16)o->countdown >= lim) {
        o->state_2++;
        func_80138070(o, o->active_flag_2);
        select_box_tables_slot00_slot00(o);
    } else {
        ref_other.p = o->parent;
        if (((Slot00Obj *)o)->countdown != ((ref_other.p->state_0 << 24) | (ref_other.p->state_1 << 16) | (ref_other.p->state_2 << 8))) {
            o->state_0 = 3;
            o->state_1 = 0;
            o->state_2 = 0;
            o->state_3 = 0;
        }
        if (game_state.pause_flag_2 != 0) {
            o->active_count = 2;
        }
        ((Slot00Obj *)o)->b2_state--;
        if (((Slot00Obj *)o)->b2_state & 0x80) {
            ((Slot00Obj *)o)->b2_state = 1;
            spawn_child_object_at_table_offset_slot00(o);
        }
        advance_sequence_checked(o);
    }
}
