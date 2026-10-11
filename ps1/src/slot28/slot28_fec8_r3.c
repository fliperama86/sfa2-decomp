/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80020608_slot28(Object *o);

void func_80020408_slot28(Object *obj) {
    func_80020608_slot28(obj);
    if (obj->pos_x >= 0x80) {
        obj->state_3 = 1;
        obj->pos_x = 0x80;
        obj->state_1++;
    }
}

void func_8002045c_slot28(Object *obj) {
    func_80020608_slot28(obj);
    if (obj->pos_x >= 0x89) {
        obj->pos_x = 0x88;
        obj->vel_x = 0x4000;
        obj->vel_y = -0x4000;
        obj->accel_x = 0;
        obj->accel_y = -0x400;
        obj->state_1++;
        func_80130768(obj, 1, (SequenceStep **)obj->box_tables);
    }
}

void func_800204d4_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;

    if (*(u8 *)&obj->field_3a == 0) {
        func_80020608_slot28(o);
        if (o->pos_y >= 0x140) {
            o->state_0++;
        }
    }
}
