/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4ea4_slot04_09(Object *o, Object *unused) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p;
    Object *q;

    if ((s16)o->step_flags & 0x8000) {
        p = (Object *)func_8011f1e0();
        if (p == 0) {
            return;
        }
        p->field_00 = 1;
        p->field_02 = 0xc;
        p->field_03 = 0xc;
        p->field_0e = o->field_0e;
        p->field_0c = o->field_0c;
        p->field_0d = o->field_0d;
        p->field_1c = o->field_1c;
        p->field_26 = o->field_26;
        p->field_3c = o;
        p->field_08 = 0x20;
        p->field_90 = o->field_90;
        p->field_98 = o->field_98;
        p->field_9c = o->field_9c;
        p->field_7a = o->field_7a;
        p->field_7c = o->field_7c;
        p->field_66 = o->side;
        o->field_160 = 0x3c;
        obj->field_330 = (u32)p;
        o->field_46 = 0;
        o->state_3++;
        q = o->other;
        obj->field_334 = q->field_0c;
        obj->field_335 = q->field_0d;
        play_sound_at(o, o->side ^ 1, 0x31a);
        play_side_sound_at(o, o->side, 9);
        play_side_sound_at(o, o->side, 0x10);
        func_801307e0(o, 0x19);
    } else {
        update_sequence(o);
    }
    set_position_from_other_box(o);
}
