/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;

void func_800795b4_slot2b(Object *obj);
void func_800795f8_slot2b(Object *obj);
void func_800796e4_slot2b(Object *obj);
void func_80079594_slot2b(Object *obj);

void func_800794e0_slot2b(Object *o) {
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->ground_y < o->pos_y) {
        o->state_0++;
        o->pos_y = (u16)data_8007ef34_slot2b->ground_y;
        o->pos_y_frac = 0;
        func_800795f8_slot2b(o);
        func_800796e4_slot2b(o);
        func_800796e4_slot2b(o);
    } else {
        advance_sequence_checked(o);
    }
}

void func_80079574_slot2b(Object *o) {
    func_80079594_slot2b(o);
}

void func_80079594_slot2b(Object *o) {
    func_8011f240((Slab172 *)o);
}

void func_800795b4_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;
    obj->field_10 += o->vel_x;
    o->vel_x += o->accel_x;
    obj->field_14 -= o->vel_y;
    o->vel_y += o->accel_y;
}
