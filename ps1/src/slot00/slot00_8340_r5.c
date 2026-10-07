/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void func_80078b50_slot00(Object *obj);
extern void func_80078bf4_slot00(Object *obj);
extern void func_80078c8c_slot00(Object *obj);
extern void func_80078d78_slot00(Object *obj);

void func_8007881c_slot00(Object *obj) {
    func_80078b50_slot00(obj);
    func_80078bf4_slot00(obj);
    func_80078d78_slot00(obj);
    func_80131094(obj);
}

void func_8007885c_slot00(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_04 = obj->field_04 + 1;
    }
    func_80131094(obj);
}

void func_800788a0_slot00(Object *obj) {
    obj->field_48 = obj->field_48 - 1;
    if (obj->field_48 & 0x80) {
        obj->field_48 = (func_80151184() & 0x1f) + 8;
        func_80078c8c_slot00(obj);
    }
    func_80078d78_slot00(obj);
    func_80131094(obj);
}

void func_8007890c_slot00(Object *obj) {
    Object *p = obj->field_3c;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_04 = obj->field_04 + 1;
    }
    func_80131094(obj);
}
