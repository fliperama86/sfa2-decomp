/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;

void func_800795b4_slot2b(Object *o);

/*
 * What it does (inferred, not an original name): an update step of an
 * object that falls toward a floor line given by another object (the one
 * data_8007ef34_slot2b points at). When the floor's field_70 is less than
 * the object's pos_y the object has landed: it is set on the floor, its
 * state is reset and a sequence is started. Otherwise func_80131094 runs.
 */
void func_800793d4_slot2b(Object *o) {
    s16 y;

    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->field_70 < o->pos_y) {
        o->state_1++;
        /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        do {
            y = data_8007ef34_slot2b->field_70;
            o->field_98 = data_80172a48;
            o->pos_y_frac = 0;
            o->field_0c = 0;
            o->field_0d = 0;
            o->field_90 = (void *)0x800fb100;
            o->pos_y = y;
            o->field_9c = data_80173c9c;
        } while (0);
        func_80130700(o, data_8017c850);
        func_801204f4(o, data_8007ef34_slot2b->side, 0x10);
    } else {
        func_80131094(o);
    }
}
