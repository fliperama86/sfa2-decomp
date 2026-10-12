/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800267bc_slot27[])(Object *);
extern SequenceStep *data_800268b0_slot27[];
void func_8001188c_slot27(Object *obj);

void func_80011660_slot27(Object *object) {
    data_800267bc_slot27[object->state_1](object);
}

void func_800116a0_slot27(Object *obj) {
    Object *p = obj->other;
    int k = obj->variant_index;

    if (k != obj->table_index) {
        obj->table_index = k;
    }
    k = p->kind;
    if (k != obj->variant_index) {
        obj->variant_index = k;
        func_8001188c_slot27(obj);
        func_80130768(obj, obj->variant_index, data_800268b0_slot27);
    }
    k = p->side;
    if ((game_state.mode >> k) & 1) {
        obj->state_1 = obj->state_1 + 1;
        obj->active_state = p->field_d4;
        func_8001188c_slot27(obj);
    }
    advance_sequence_checked(obj);
}
