/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c09d0_slot04_03[])(Object *, Object *);

void func_801b3d44_slot04_03(Object *obj, Object *p) {
    data_801c09d0_slot04_03[obj->field_06](obj, p);
    func_8011ffdc(obj);
}

void func_801b3d98_slot04_03(Object *obj, Object *unused) {
    obj->field_06 = obj->field_06 + 1;
    func_80138070(obj, 6);
}

void func_801b3dc4_slot04_03(Object *obj, Object *unused) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        if (((s16)obj->field_3a & 0x8000) == 0) {
            obj->field_04++;
        }
        func_80131094(obj);
    }
}

void func_801b3e20_slot04_03(Object *obj, Object *unused) {
    ref_other.p = obj->field_3c;
    ref_other.p->field_240--;
    if (ref_other.p->field_240 == 0) {
        ref_other.p->field_14c = 0;
    }
    if (obj->field_03 == 0) {
        ref_other.p->field_225 = 0;
    }
    func_8011f14c((Slab172 *)obj);
}
