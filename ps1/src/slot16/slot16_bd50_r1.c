/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8007e870_slot16[])(Object *);
extern void func_8007bf74_slot16(Object *obj);
extern void func_8007bf98_slot16(Object *obj);
extern void func_8007bfb8_slot16(Object *obj);

void func_8007bd50_slot16(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_8007bf98_slot16(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_8007bdb4_slot16(Object *obj) {
    obj->field_157 = 1;
    data_8007e870_slot16[obj->state_3](obj);
}

void func_8007bdf8_slot16(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->field_0b = obj->field_158;
    func_8007bf74_slot16(obj);
}

void func_8007be28_slot16(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_8007bfb8_slot16(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
