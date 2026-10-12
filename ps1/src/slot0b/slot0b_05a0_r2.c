/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e08d8_slot0b(ModObj *obj);
void func_801e0578_slot0b(Object *obj, u8 a);

void func_801e06d0_slot0b(ModObj *obj) {
    int t;
    obj->step_accum += 0x10;
    obj->counter_a += 1;
    obj->counter_b += 1;
    if (data_80190468.p->phase_state != 1) {
        t = obj->countdown_timer;
        t--;
        obj->countdown_timer = t;
        if ((s16)t >= 0) {
            return;
        }
    }
    obj->step_accum = 0;
    obj->counter_a = 0;
    obj->counter_b = 0;
    obj->substate_index += 1;
    func_801e08d8_slot0b(obj);
}

void func_801e0760_slot0b(ModObj *obj) {
    if (data_80190468.p->phase_state == 2) {
        obj->countdown_timer = 0xf;
        obj->substate_index += 1;
        ref_other.p = obj->owner_object;
        func_801e0578_slot0b((Object *)obj, func_80125b18(ref_other.p->kind));
        if (obj->child != 0) {
            obj->child->mode = 2;
        }
    }
}
