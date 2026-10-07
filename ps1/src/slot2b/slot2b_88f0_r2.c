/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;
extern ObjectFn data_8007edc8_slot2b[];

void func_80078a14_slot2b(Object *o) {
    o->field_0e = data_8007ef34_slot2b->field_0e;
    o->field_4c = (((Slot2bObj *)data_8007ef34_slot2b->other)->field_10 - ((Slot2bObj *)data_8007ef34_slot2b)->field_10) >> 7;
    o->field_54 = 0;
    o->field_50 = 0x30000;
    o->field_58 = 0xffff6000;
    func_80138070(o, 4);
}

void func_80078a7c_slot2b(Object *o) {
    o->field_0e = data_8007ef34_slot2b->field_0e;
    o->field_4c = 0;
    o->field_54 = 0;
    o->field_50 = 0x60000;
    o->field_58 = 0xffff6000;
    func_80138070(o, 4);
}

void func_80078ac8_slot2b(Object *o) {
    if (game_state.field_6a != 0) {
        o->field_04 = o->field_04 + 1;
    } else {
        if (game_state.field_65 == 0) {
            data_8007edc8_slot2b[o->field_03](o);
        }
        func_80120028(o);
    }
}
