/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u16 func_801b1e04_slot04_0a(Object *obj);
void func_801b1c00_slot04_0a(Object *obj);

void func_801b1afc_slot04_0a(Object *obj) {
    if (func_801b1e04_slot04_0a(obj)) {
        func_801b1c00_slot04_0a(obj);
    } else if (obj->vel_y > 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        func_801307e0(obj, (obj->direction_index >> 1) + 0x25);
    }
}

void func_801b1b7c_slot04_0a(Object *obj) {
    if (func_801b1e04_slot04_0a(obj)) {
        func_801b1c00_slot04_0a(obj);
    } else if (obj->active_flag == 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->other->field_6b = 0x10;
        func_801307e0(obj, 0x23);
    }
}

void func_801b1c00_slot04_0a(Object *obj) {
    obj->state_3 = 7;
    func_801209c4(obj);
    obj->pos_y_frac = 0;
    obj->pos_y = (u16)obj->ground_y;
    func_801307e0(obj, 0x48);
}
