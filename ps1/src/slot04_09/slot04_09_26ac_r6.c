/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);
void func_801b2d6c_slot04_09(Object *o);

void func_801b2bf0_slot04_09(Object *obj) {
    func_801b2d6c_slot04_09(obj);
    func_801b2858_slot04_09(obj);
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y_frac = 0;
        obj->pos_y = obj->ground_y;
        obj->other->active_state = 0;
        if (get_random_byte() & 1) {
            func_801307e0(obj, 0x35);
        } else {
            func_801307e0(obj, 0x36);
        }
    }
}

void func_801b2c90_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        Object *p = obj->other;
        p->field_0c = ((Slot04aObj *)obj)->field_334;
        p->image_row = ((Slot04aObj *)obj)->field_335;
        obj->active_flag_3 = 0;
        obj->state_3 = 0;
        func_801312b8(obj);
    } else {
        func_801b2d6c_slot04_09(obj);
        update_sequence(obj);
    }
}

void func_801b2d08_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        Object *p = obj->other;
        p->field_0c = ((Slot04aObj *)obj)->field_334;
        p->image_row = ((Slot04aObj *)obj)->field_335;
        obj->active_flag_3 = 0;
        obj->state_3 = 0;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}
