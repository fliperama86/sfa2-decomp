/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80027d0c_slot27[];
extern SequenceStep *data_800280bc_slot27[];

void func_800149d0_slot27(Object *obj) {
    obj->field_01 = 1;
    obj->spawn_x = 0x40;
    obj->spawn_y = 0x1e0;
    obj->field_0d = 0;
    if (obj->step_flags & 1) {
        game_state.field_06 = 0;
    }
    if ((s16)obj->step_flags & 0x8000) {
        func_8011f240((Slab172 *)obj);
    }
}

void func_80014a34_slot27(Object *obj) {
    data_80027d0c_slot27[obj->state_1](obj);
}

void func_80014a74_slot27(Object *obj) {
    Object *other = obj->other;
    if (((game_state.mode | game_state.field_07) >> other->side) & 1) {
        obj->field_0d = 0x1b;
        obj->pos_x = 0x1b0;
        obj->state_1++;
        if (other->side != 0) {
            obj->pos_x = 0x250;
        }
        func_80130768(obj, other->kind, data_800280bc_slot27);
        obj->field_66 = other->kind;
        func_8011ffdc(obj);
    }
}
