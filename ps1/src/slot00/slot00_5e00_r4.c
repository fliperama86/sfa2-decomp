/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079164_slot00[];
extern ObjectFn data_8007916c_slot00[];

void func_8007620c_slot00(Object *obj) {
    data_80079164_slot00[obj->state_3](obj);
}

void func_8007624c_slot00(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    func_80130dc0(obj);
}

void func_80076284_slot00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_800762ec_slot00(Object *obj) {
    data_8007916c_slot00[obj->state_3](obj);
}
