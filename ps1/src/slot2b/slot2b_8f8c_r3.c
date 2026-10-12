/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;
extern Vec4 data_8007ee14_slot2b[];
extern ObjectFn data_8007ee54_slot2b[];

void func_800795b4_slot2b(Object *obj);

void func_80079208_slot2b(Object *o) {
    s32 *p = (s32 *)&data_8007ee14_slot2b[game_state.field_24 & 3];
    o->vel_x = *p++;
    o->accel_x = *p++;
    o->vel_y = *p;
    o->accel_y = p[1];
}

void func_80079254_slot2b(Object *o) {
    data_8007ee54_slot2b[o->state_1](o);
}

void func_80079294_slot2b(Object *o) {
    Object *p;
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->ground_y < o->pos_y) {
        o->state_1++;
        p = data_8007ef34_slot2b;
        o->pos_y = (u16)p->ground_y;
        o->pos_y_frac = 0;
        o->vel_x = (((Slot2bObj *)p->other)->field_10 - ((Slot2bObj *)p)->field_10) >> 7;
        o->vel_y = 0x40000;
        o->accel_y = -0x8000;
    }
    func_80131094(o);
}
