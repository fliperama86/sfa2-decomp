/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_800280bc_slot27[];

void func_80014b1c_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Object *other = o->other;
    if (other->kind != o->sound_side) {
        o->sound_side = other->kind;
        func_80130768(o, other->kind, data_800280bc_slot27);
    }
    func_8011ffdc(o);
    if (data_8018f5a0->substep_index >= 3 && game_state.mode != 3) {
        *(u16 *)&o->pos_y += 8;
        if (*(u16 *)&o->pos_y >= 0x2c1) {
            o->field_01 = 0;
            o->state_0++;
        }
    }
}

void func_80014be8_slot27(Object *obj) {
    obj->state_0++;
}

void func_80014bfc_slot27(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
