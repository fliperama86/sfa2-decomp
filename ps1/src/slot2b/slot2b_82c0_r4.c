/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef30_slot2b;
void func_80078734_slot2b(Object *o);
void func_8011f38c(Object *o);
void func_8007878c_slot2b(Object *o, u8 a);

void func_80078654_slot2b(Object *o) {
    Object *p;
    u8 t;

    o->field_01 = 0;
    p = data_8007ef30_slot2b;
    if (o->field_03 != p->kind) {
        func_80078734_slot2b(o);
    } else {
        t = p->frame->field_09;
        if (t == 0) {
            o->field_48 = 0;
        } else {
            o->pos_x = p->pos_x;
            o->pos_y = p->pos_y;
            o->field_0b = p->field_0b;
            o->field_0c = data_8007ef30_slot2b->field_0c;
            o->field_0d = data_8007ef30_slot2b->field_0d;
            o->field_01 = 1;
            if (o->field_48 != t) {
                func_8007878c_slot2b(o, t);
            } else {
                func_80131094(o);
            }
        }
    }
}

void func_80078734_slot2b(Object *o) {
    o->field_04++;
}

void func_80078748_slot2b(Object *o) {
    Slot2bObj *p = (Slot2bObj *)data_8007ef30_slot2b;

    if ((u32)o == p->field_2c) {
        p->field_2c = 0;
    }
    func_8011f38c(o);
}

void func_80078784_slot2b(Object *o) {
    o->field_48 = 0;
}
