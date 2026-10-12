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
    advance_sequence_checked(obj);
}

void func_8007885c_slot00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0 = obj->state_0 + 1;
    }
    advance_sequence_checked(obj);
}

void func_800788a0_slot00(Object *obj) {
    obj->variant_index = obj->variant_index - 1;
    if (obj->variant_index & 0x80) {
        obj->variant_index = (get_random_byte() & 0x1f) + 8;
        func_80078c8c_slot00(obj);
    }
    func_80078d78_slot00(obj);
    advance_sequence_checked(obj);
}

void func_8007890c_slot00(Object *obj) {
    Object *p = obj->parent;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0 = obj->state_0 + 1;
    }
    advance_sequence_checked(obj);
}
