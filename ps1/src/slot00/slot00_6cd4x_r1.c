/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800797f4_slot00[];
void select_box_tables_slot00_slot00(Object *obj);

void func_80076cd4_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    int v;
    u8 t;
    int w;

    o->countdown = (s16)o->countdown - 1;
    if ((s16)o->countdown == 0) {
        o->active_count = 1;
        o->state_0 = 1;
        o->state_2 = 1;
        o->state_1 = 0;
        o->state_3 = 0;
        v = -o->vel_x;
        o->vel_x = v;
        if (!(obj->step_count & 1)) {
            o->vel_x = (v >> 1) + v;
            if (o->active_flag_2 == 0) {
                w = o->table_index_3 + 1;
                t = w;
                if (w >= 8) {
                    t = 7;
                }
                o->table_index_3 = t;
                select_box_tables_slot00_slot00(o);
            }
        }
    }
    advance_sequence_checked(o);
}
