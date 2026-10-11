/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80077ef0_slot00(Object *obj) {
    Slot00Rec7ef0 *rec;
    s16 v;
    if (game_state.field_6a != 0) goto fail;
    {
        v = (obj->field_1e + 1) & 3;
        obj->field_1e = v;
        if (v == 0) {
            rec = (Slot00Rec7ef0 *)((u8 *)obj + 0x40);
        } else if (v == 1) {
            rec = (Slot00Rec7ef0 *)((u8 *)obj + 0x50);
        } else if (v == 2) {
            rec = (Slot00Rec7ef0 *)((u8 *)obj + 0x60);
        } else {
            rec = (Slot00Rec7ef0 *)((u8 *)obj + 0x20);
        }
        obj->sequence = rec->seq;
        obj->step_timer = rec->field_04;
        obj->step_flags = rec->field_06;
        obj->sequence = obj->sequence + 1;
        if ((s16)rec->field_06 >= 0) goto ok;
    }
fail:
    obj->state_0 = 2;
    obj->field_01 = 0;
    return;
ok:
    {
            obj->step_timer = obj->sequence->duration;
            obj->step_flags = obj->sequence->flags;
            rec->seq = obj->sequence;
            rec->field_04 = obj->step_timer;
            rec->field_06 = obj->step_flags;
            obj->field_80 = 1;
            rec->field_0c += rec->field_08;
            rec->field_0e -= rec->field_0a;
            obj->pos_x = rec->field_0c;
            obj->pos_y = rec->field_0e;
            func_80120028(obj);
    }
}

void func_8007801c_slot00(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
