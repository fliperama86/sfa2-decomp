/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001791c_slot27(void);
extern ObjectFn data_80028e68_slot27[];
extern Slot27Recb380 data_8002b380_slot27;

void func_80016790_slot27(Object *obj) {
    func_8011ffdc(obj);
}

void func_800167b0_slot27(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_800167d0_slot27(Object *obj, s16 *p) {
    int t;
    data_8002b380_slot27.ptr = p;
    t = p[1];
    data_8002b380_slot27.count = t;
    data_8002b380_slot27.frame = (u16)p[0];
    obj->field_0d = (u8)data_8002b380_slot27.frame & 0x1f;
}

void func_80016810_slot27(Object *obj) {
    int t;
    data_8002b380_slot27.count -= 1;
    if (data_8002b380_slot27.count == 0) {
        data_8002b380_slot27.ptr += 2;
        if (*(int *)data_8002b380_slot27.ptr < 0) {
            data_8002b380_slot27.ptr = (u16 *)*(int *)data_8002b380_slot27.ptr;
        }
        t = ((s16 *)data_8002b380_slot27.ptr)[1];
        data_8002b380_slot27.count = t;
        data_8002b380_slot27.frame = data_8002b380_slot27.ptr[0];
        obj->field_0d = (u8)data_8002b380_slot27.frame & 0x1f;
    }
}

void func_800168a0_slot27(Object *obj) {
    data_80028e68_slot27[obj->field_04](obj);
    func_8001791c_slot27();
}
