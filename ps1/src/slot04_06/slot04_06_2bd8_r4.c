/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);
void func_801b4850_slot04_06(Object *obj);

void func_801b3084_slot04_06(Object *obj) {
    int v;

    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->field_17b = 1;
    obj->state_3++;
    func_80141f28(obj, 8);
    func_80138ae8(&game_state, obj);
    obj->vel_x = 0x40000;
    obj->vel_y = 0x28000;
    obj->accel_x = 0;
    obj->accel_y = 0xffff6000;
    obj->field_45 = 1;
    obj->field_159 = 1;
    v = 0x48;
    if (obj->field_49 == 0) {
        v = 0x2a;
    }
    func_801307e0(obj, v);
}

void func_801b3134_slot04_06(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        ((Slot04aObj *)obj)->field_1c3 = 0x10;
    } else {
        func_80130efc(obj);
    }
}

void func_801b3178_slot04_06(Object *obj) {
    func_801b4818_slot04_06(obj);
    func_801b4850_slot04_06(obj);
    if (((Slot04aObj *)obj)->field_1c3 != 0) {
        if (--((Slot04aObj *)obj)->field_1c3 == 0) goto go;
    }
    if (obj->pos_y > obj->field_70) {
go:
        obj->state_3++;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        func_80130efc(obj);
    }
}
