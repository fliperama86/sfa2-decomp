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
    o->field_4c = *p++;
    o->field_54 = *p++;
    o->field_50 = *p;
    o->field_58 = p[1];
}

void func_80079254_slot2b(Object *o) {
    data_8007ee54_slot2b[o->field_05](o);
}

void func_80079294_slot2b(Object *o) {
    Object *p;
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->field_70 < o->pos_y) {
        o->field_05++;
        p = data_8007ef34_slot2b;
        o->pos_y = (u16)p->field_70;
        o->field_14 = 0;
        o->field_4c = (((Slot2bObj *)p->other)->field_10 - ((Slot2bObj *)p)->field_10) >> 7;
        o->field_50 = 0x40000;
        o->field_58 = -0x8000;
    }
    func_80131094(o);
}
