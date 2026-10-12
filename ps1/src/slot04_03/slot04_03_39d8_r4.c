/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c09d0_slot04_03[])(Object *, Object *);

void func_801b3d44_slot04_03(Object *obj, Object *p) {
    data_801c09d0_slot04_03[obj->state_2](obj, p);
    func_8011ffdc(obj);
}

void func_801b3d98_slot04_03(Object *obj, Object *unused) {
    obj->state_2 = obj->state_2 + 1;
    func_80138070(obj, 6);
}

void func_801b3dc4_slot04_03(Object *obj, Object *unused) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        if (((s16)obj->step_flags & 0x8000) == 0) {
            obj->state_0++;
        }
        advance_sequence_checked(obj);
    }
}

void func_801b3e20_slot04_03(Object *obj, Object *unused) {
    ref_other.p = obj->parent;
    ref_other.p->link_flags--;
    if (ref_other.p->link_flags == 0) {
        ref_other.p->linked_object_3 = 0;
    }
    if (obj->table_index == 0) {
        ref_other.p->field_225 = 0;
    }
    func_8011f14c((Slab172 *)obj);
}
