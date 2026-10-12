/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e1554_slot0b(Slot0bObj *obj);

void func_801e14b0_slot0b(Slot0bObj *obj) {
    int v;
    if (data_80190468.p->phase_state == 2) {
        v = obj->countdown_timer - 1;
        obj->countdown_timer = v;
        if ((s16)v < 0) {
            obj->handler_index++;
        }
        func_801e1554_slot0b(obj);
    }
}

void func_801e1514_slot0b(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e1534_slot0b(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e1554_slot0b(Slot0bObj *obj) {
    obj->fx += obj->fx_step;
    obj->fx_step += obj->fx_step_accel;
    obj->fy += obj->fy_step;
    obj->fy_step += obj->fy_step_accel;
}
